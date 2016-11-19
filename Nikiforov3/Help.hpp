#pragma once
#include <unordered_map>
#include <map>

using namespace std;

// check whether index is unique
// map
template<typename T, typename ... Types> 
struct is_map { static constexpr bool value = false; };

template<typename ... Types>
struct is_map<map<Types...>> { static constexpr bool value = true;};
// unordered_map
template<typename T, typename ... Types>
struct is_unordered_map { static constexpr bool value = false; };

template<typename ... Types>
struct is_unordered_map<unordered_map<Types...>> { static constexpr bool value = true; };
// is_unique_index
template <typename T>
struct is_unique_index { static constexpr bool value = is_map<T>::value || is_unordered_map<T>::value; };

//remove key
template<typename IndexType,typename ... Args>
void remove_key(IndexType& index, Tid tid, Args ... args) {
	auto t = make_tuple(args...);
	if (is_unique_index<IndexType>::value) {
		auto it = index.find(t);
		if (it != index.end()) {
			index.erase(it);
			return;
		} 
	} else {
		for (auto it = index.equal_range(t); it.first != it.second; ++it.first) {
			if (it.first->second == tid) {
				index.erase(it.first);
				return;
			}
		}
	}
	throw "remove_key::not found";
}

//replace key
template<typename IndexType,typename ... Args>
void replace_key(IndexType& index, Tid old_tid, Tid new_tid, Args ... args) {
	auto t = make_tuple(args...);
	if (is_unique_index<IndexType>::value) {
		auto it = index.find(t);
		if (it != index.end()) {
			it->second = new_tid;
			return;
		} 
	} else {
		for (auto it = index.equal_range(t); it.first != it.second; ++it.first) {
			if (it.first->second == old_tid) {
				it.first->second = new_tid;
				return;
			}
		}
	}
	throw "replace_key::not found";
}

//check key
template<typename IndexType,typename ... Args>
void check_key(IndexType& index, Args ... args) {
	if (is_unique_index<IndexType>::value) {
		auto t = make_tuple(args...);
		auto it = index.find(t);
		if (it != index.end()) {
			throw "check_key::duplicate in unique index";
		} 
	} 
}

//insert key 
//  If you have multiple unique indices:
//  - check_key for each influenced unique index
//  - insert_key<false> for each influenced index (both unique and non-unique)
//  Idea: if duplicate value appears in second+ index (=>exception), 
//  indices are still consistent. Without precheck first index will contain
//  new data, while failed index won't
template<bool check = true,typename IndexType,typename ... Args>
void insert_key(IndexType& index, Tid tid, Args ... args) {
	auto t = make_tuple(args...);
	if (check && is_unique_index<IndexType>::value) {
		auto it = index.find(t);
		if (it != index.end()) {
			throw "insert_key::duplicate in unique index";
		} 
	} 
	index.insert(make_pair(t, tid));
}