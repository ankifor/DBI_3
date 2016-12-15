#pragma once
#include <iostream>
#include <tuple>
#include <vector>
#include "block_storage.h"

template<typename _Tp>
class Dummy_UpdFunc {
public:
	void operator()(_Tp& left, const _Tp& right) {}
};

using namespace std;
template<
	typename _Key, 
	typename _Tp,
	typename _Hash = hash<_Key>,
	typename _Pred = equal_to<_Key>,
	bool _Unique = true,
	typename _UpdFunc = Dummy_UpdFunc<_Tp> 
	>
class My_Hash {
public:
	using Entry = tuple<_Key, _Tp, void*>;
	Block_Storage<Entry, 1024*400> _storage;
	
private:
	
	static const size_t _Key_N = 0;
	static const size_t _Tp_N = 1;
	static const size_t _Next_N = 2;
	constexpr double _target_load_factor() {return 0.75;}
	static const size_t _min_log2_n_buckets = 10;
	size_t _log2_n_buckets;
	size_t _size_threshold;
	
	vector<Entry*> _hash_table;
	size_t _size;
	_UpdFunc _upd;
	
	void calculate_next_size_threshold() {
		_size_threshold = size_t(_target_load_factor() * double(_hash_table.size()));
	}

	void rehash_if_full() {
		if (_size < _size_threshold) return;
		size_t new_log2_size = _log2_n_buckets + 1;
		rehash<false>(new_log2_size);
	}
	size_t get_bucket(_Key key) {
		_Hash h;
		return h(key) & ((1ull << _log2_n_buckets)-1);
	}

public:
	class Iterator;

	My_Hash() : _log2_n_buckets(_min_log2_n_buckets),_size(0)
	{
		_hash_table.resize(1 << _log2_n_buckets, 0);
		calculate_next_size_threshold();
	}
	
	~My_Hash() { clear(); }
	
	void clear() {
		_storage.clear();
		_hash_table.clear();
		_size = 0;
		_log2_n_buckets = -1;
	}

	size_t size() const { return _size;}
	
	Iterator insert(_Key key, _Tp val) {
		size_t bucket = get_bucket(key);
		Entry** current = &_hash_table[bucket];
		//if unique, then insert to the beginning of the bucket
		//else insert before the first item with the same key
		if (!_Unique) {
			_Pred eq;
			while (*current != nullptr) {
				if (eq(key, get<_Key_N>(**current))) break;
				current = reinterpret_cast<Entry**>(&get<_Next_N>(**current));
			}
		}
		*current = _storage.insert(make_tuple(
			key, val, reinterpret_cast<void*>(*current)));
		++_size;
		
		rehash_if_full();
		
		return Iterator(*this, bucket, *current);
	}
	
	Iterator modify(_Key key, _Tp val) {
		_Pred eq;
		
		size_t bucket = get_bucket(key);
		Entry** current = &_hash_table[bucket];
		//search for key
		while (*current != nullptr && !eq(key, get<_Key_N>(**current))) {
			current = reinterpret_cast<Entry**>(&get<_Next_N>(**current));
		}
		if (!_Unique || *current == nullptr) {
			//insert
			*current = _storage.insert(make_tuple(key, val, (void*) *current));
			++_size;
		} else {
			//update
			_upd(get<_Tp_N>(**current),val);
		}
		
		rehash_if_full();
		return Iterator(*this, bucket, *current);
	}
	
	Iterator find(_Key x) {
		size_t bucket = get_bucket(x);
		Entry* current = _hash_table[bucket];
		_Pred eq;
		while (current != nullptr && !eq(x, get<_Key_N>(*current))) {
			current = reinterpret_cast<Entry*>(get<_Next_N>(*current));
		}
		
		return Iterator(*this, bucket, current);
	}
	
	pair<Iterator,Iterator> equal_range(_Key&& x) {
		_Hash h;
		
		size_t bucket = get_bucket(x);
		Entry* left = _hash_table[bucket];
		_Pred eq;
		while (left != nullptr && !eq(x, get<_Key_N>(*left))) {
			left = reinterpret_cast<Entry*>(get<_Next_N>(*left));
		}
		
		Entry* right = left;
		
		while (right != nullptr && eq(x, get<_Key_N>(*right))) {
			right = reinterpret_cast<Entry*>(get<_Next_N>(*right));
		}
		
		return make_pair(
			 Iterator(*this, bucket, left)
			,Iterator(*this, bucket, right));
	}
	
	template<bool with_update>
	void build_from_storage() {
		static_assert(_Unique || !with_update, "_Unique=false and with_update=true are incompatible");
		size_t sz = size_t(double(_storage.size()) / _target_load_factor() );
		size_t tmp = size_t(log2(sz));
		
		rehash<with_update>(tmp);
	}
	
	template<bool with_update>
	void rehash(size_t new_log2_n_buckets) {
		assert(!_Unique || !with_update);
		if (new_log2_n_buckets < _min_log2_n_buckets) 
			new_log2_n_buckets = _min_log2_n_buckets;
		//_hash_table.clear();
		_hash_table.resize(1ull << new_log2_n_buckets);
		_log2_n_buckets = new_log2_n_buckets;
		calculate_next_size_threshold();
		memset(_hash_table.data(), 0, _hash_table.size()*sizeof(Entry*));
		_size = 0;
		
		_Pred eq;
		size_t bucket = 0;
		Entry** current = nullptr;
		for (auto it = _storage.begin(); it != _storage.end(); ++it) {
			bucket = get_bucket(get<_Key_N>(*it));
			current = &_hash_table[bucket];
			//search for key
			if (!_Unique || with_update) {
				while (*current != nullptr) {
					if (eq(get<_Key_N>(*it), get<_Key_N>(**current))) break;
					current = reinterpret_cast<Entry**>(&get<_Next_N>(**current));
				}
			}
			
			if (!_Unique || !with_update || *current == nullptr) {
				//insert
				get<_Next_N>(*it) = *current;
				*current = it.get_data();
				++_size;
			} else {
				//update
				_upd(get<_Tp_N>(**current),get<_Tp_N>(*it));
			}
		}
	}
	

//Iterator description
public:
	
	class Iterator {
	public:
		Iterator(My_Hash& my_hash, size_t bucket, My_Hash::Entry* entry) 
			: _hash(my_hash), _bucket(bucket), _entry(entry) {}
			
		~Iterator() {}
		
		bool operator==(const Iterator& it) const { return _entry==it._entry; }
		bool operator!=(const Iterator& it) const { return _entry!=it._entry; }
		
		Iterator& operator++() { //prefix increment
			if (get<_Next_N>(*_entry) != nullptr) {
				_entry = reinterpret_cast<Entry*>(get<_Next_N>(*_entry));
			} else {
				_entry = nullptr;
				while (++_bucket < _hash._hash_table.size()) {
					_entry = _hash._hash_table[_bucket];
					if (_entry != nullptr) break;
				}
			}

			return *this;
		}
		
		Entry& operator*() const { return *_entry; }
	private:
		My_Hash& _hash;
		size_t _bucket;
		Entry* _entry;
	};
	
	Iterator begin() { 
		size_t i = 0;
		for (i = 0; i < _hash_table.size() && _hash_table[i] == nullptr; ++i) {;}
		return Iterator(*this,i, (i == _hash_table.size()?nullptr:_hash_table[i]));
	}
	Iterator end() { return Iterator(*this,_hash_table.size(),nullptr); }
};



