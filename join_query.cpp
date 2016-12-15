#include <unordered_map>

#include "Types.hpp"
#include "schema_1.hpp"
#include "Help.hpp"
#include "my_hash.h"


#include <iostream>

using namespace std;

//static bool pred(Integer c_id, Integer c_d_id, Integer c_w_id) {
//	return c_id == 1 && c_d_id<=10 && c_w_id==4;
//}
static bool pred0(Varchar<16> c_last) {
	return c_last.len >= 1 && c_last.value[0] == 'B';
}



Numeric<12,4> join_query() {
	size_t n_buckets_wdc = 10000;
	size_t n_buckets_wdo = 10000;
	
	using type_key_wdc = tuple<Integer,Integer,Integer>;//w_id,d_id,c_id
	using type_val_wdc = tuple<Numeric<12,2>>;//c_balance
	unordered_map<type_key_wdc,type_val_wdc,hash_types::hash<type_key_wdc>> hash_wdc;
	hash_wdc.rehash(n_buckets_wdc);
	// filter and create hash table
	for (Tid tid = 0; tid < customer.size(); ++tid) {
		if (pred0(customer.c_last[tid])) {
		//if (pred(customer.c_id[tid],customer.c_d_id[tid],customer.c_w_id[tid])) {
			auto t_key = make_tuple(customer.c_w_id[tid], customer.c_d_id[tid], customer.c_id[tid]);
			auto t_val = make_tuple(customer.c_balance[tid]);
			hash_wdc.insert(make_pair(t_key,t_val));
		}
	}
	//cerr << "0,hash_wdc:" << hash_wdc.size() << endl;
	//cout << customer_wdc.size() << endl;
	// match order with customer_wdc and create new hash, storing pairs of TID
	using type_key_wdo = tuple<Integer,Integer,Integer>;//w_id,d_id,o_id
	using type_val_wdo = tuple<Numeric<12,2>, Numeric<2,0>>;//c_balance * o_ol_cnt
	unordered_map<type_key_wdo,type_val_wdo,hash_types::hash<type_key_wdo>> hash_wdo;
	
	hash_wdo.rehash(n_buckets_wdo);
	
	for (Tid tid = 0; tid < order.size(); ++tid) {
		auto t_key = make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_c_id[tid]);
		auto it = hash_wdc.find(t_key);
		if (it != hash_wdc.end()) {
			auto t_new = make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_id[tid]);
			//auto t_val = tuple_cat(it->second, make_tuple(order.o_ol_cnt[tid]));
			auto t_val = make_tuple(get<0>(it->second), order.o_ol_cnt[tid]);
			hash_wdo.insert(make_pair(t_new, t_val));
		}
	}
	//cerr << "0,hash_wdo:" << hash_wdo.size() << endl;
	//cout << customer_order_wdo.size() << endl;
	// match orderline with customer_order_wdo and calculate sum
	Numeric<12,4> sum = 0;
	size_t count = 0;
	for (Tid tid = 0; tid < orderline.size(); ++tid) {
		auto t_key = make_tuple(orderline.ol_w_id[tid],orderline.ol_d_id[tid], orderline.ol_o_id[tid]);
		auto it = hash_wdo.find(t_key);
		if (it != hash_wdo.end()) {
			auto x = get<0>(get<1>(*it)) * get<1>(get<1>(*it)).cast<12,2>();
			//select sum(ol_quantity*ol_amount-c_balance*o_ol_cnt)
			++count;
			sum 
				+= orderline.ol_quantity[tid].cast<12,2>() * orderline.ol_amount[tid].cast<12,2>()
				- x;
		}
	}
	//cerr << "0,final:" << count << endl;
	return sum;


}



Numeric<12,4> join_query1() {
	using type_key_wdc = tuple<Integer,Integer,Integer>;//w_id,d_id,c_id
	using type_val_wdc = tuple<Numeric<12,2>>;//c_balance
	My_Hash<type_key_wdc, type_val_wdc, hash_types::hash<type_key_wdc>
		, equal_to<type_key_wdc>, true> hash_wdc;
	// filter and create hash table
	for (Tid tid = 0; tid < customer.size(); ++tid) {
		if (pred0(customer.c_last[tid])) {
		//if (pred(customer.c_id[tid],customer.c_d_id[tid],customer.c_w_id[tid])) {
			hash_wdc._storage.insert(make_tuple(
				 make_tuple(customer.c_w_id[tid], customer.c_d_id[tid], customer.c_id[tid])
				,make_tuple(customer.c_balance[tid])
				,nullptr));
		}
	}
	hash_wdc.build_from_storage<false>();
	//cerr << "1,hash_wdc:" << hash_wdc.size() << endl;
	
	//cout << customer_wdc.size() << endl;
	// match order with customer_wdc and create new hash, storing pairs of TID
	using type_key_wdo = tuple<Integer,Integer,Integer>;//w_id,d_id,o_id
	using type_val_wdo = tuple<Numeric<12,2>, Numeric<2,0>>;//c_balance * o_ol_cnt
	My_Hash<type_key_wdo, type_val_wdo, hash_types::hash<type_key_wdo>
		, equal_to<type_key_wdo>, true> hash_wdo;
	
	for (Tid tid = 0; tid < order.size(); ++tid) {
		auto it = hash_wdc.find(make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_c_id[tid]));
		if (it != hash_wdc.end()) {
			hash_wdo._storage.insert(make_tuple(
				make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_id[tid])
				, make_tuple(get<0>(get<1>(*it)),order.o_ol_cnt[tid])
				, nullptr));
		}
	}
	hash_wdc.clear();
	hash_wdo.build_from_storage<false>();
	//cerr << "1,hash_wdo:" << hash_wdo.size() << endl;
	
	//cout << customer_order_wdo.size() << endl;
	// match orderline with customer_order_wdo and calculate sum
	Numeric<12,4> sum = 0;
	size_t count = 0;
	for (Tid tid = 0; tid < orderline.size(); ++tid) {
		//if (tid == 119558) {
		//	++tid;
		//	--tid;
		//	cout << make_tuple(orderline.ol_w_id[tid],orderline.ol_d_id[tid], orderline.ol_o_id[tid]) << endl;
		//}
		
		auto it = hash_wdo.find(make_tuple(orderline.ol_w_id[tid],orderline.ol_d_id[tid], orderline.ol_o_id[tid]));
		if (it != hash_wdo.end()) {
			auto x = get<0>(get<1>(*it)) * get<1>(get<1>(*it)).cast<12,2>();
			//select sum(ol_quantity*ol_amount-c_balance*o_ol_cnt)
			++count;
			sum 
				+= orderline.ol_quantity[tid].cast<12,2>() * orderline.ol_amount[tid].cast<12,2>()
				- x
				;
		}
	}
	//cerr << "1,final:" << count << endl;
	hash_wdo.clear();
	return sum;
}


Numeric<12,4> join_query2() {
	using type_key_wdc = tuple<Integer,Integer,Integer>;//w_id,d_id,c_id
	using type_val_wdc = tuple<Numeric<12,2>>;//c_balance
	My_Hash<type_key_wdc, type_val_wdc, hash_types::hash<type_key_wdc>
		, equal_to<type_key_wdc>, true> hash_wdc;//
	// filter and create hash table
	for (Tid tid = 0; tid < customer.size(); ++tid) {
		if (pred0(customer.c_last[tid])) {
		//if (pred(customer.c_id[tid],customer.c_d_id[tid],customer.c_w_id[tid])) {
			hash_wdc.insert(
				 make_tuple(customer.c_w_id[tid], customer.c_d_id[tid], customer.c_id[tid])
				,make_tuple(customer.c_balance[tid]));
		}
	}
	//cerr << "1,hash_wdc:" << hash_wdc.size() << endl;
	
	//cout << customer_wdc.size() << endl;
	// match order with customer_wdc and create new hash, storing pairs of TID
	using type_key_wdo = tuple<Integer,Integer,Integer>;//w_id,d_id,o_id
	using type_val_wdo = tuple<Numeric<12,2>, Numeric<2,0>>;//c_balance * o_ol_cnt
	My_Hash<type_key_wdo, type_val_wdo, hash_types::hash<type_key_wdo>
		, equal_to<type_key_wdo>, true> hash_wdo;
	
	for (Tid tid = 0; tid < order.size(); ++tid) {
		auto it = hash_wdc.find(make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_c_id[tid]));
		if (it != hash_wdc.end()) {
			hash_wdo.insert(
				make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_id[tid])
				, make_tuple(get<0>(get<1>(*it)),order.o_ol_cnt[tid]));
		}
	}
	hash_wdc.clear();
	//cerr << "1,hash_wdo:" << hash_wdo.size() << endl;
	
	//cout << customer_order_wdo.size() << endl;
	// match orderline with customer_order_wdo and calculate sum
	Numeric<12,4> sum = 0;
	size_t count = 0;
	for (Tid tid = 0; tid < orderline.size(); ++tid) {
		//if (tid == 119558) {
		//	++tid;
		//	--tid;
		//	cout << make_tuple(orderline.ol_w_id[tid],orderline.ol_d_id[tid], orderline.ol_o_id[tid]) << endl;
		//}
		
		auto it = hash_wdo.find(make_tuple(orderline.ol_w_id[tid],orderline.ol_d_id[tid], orderline.ol_o_id[tid]));
		if (it != hash_wdo.end()) {
			auto x = get<0>(get<1>(*it)) * get<1>(get<1>(*it)).cast<12,2>();
			//select sum(ol_quantity*ol_amount-c_balance*o_ol_cnt)
			++count;
			sum 
				+= orderline.ol_quantity[tid].cast<12,2>() * orderline.ol_amount[tid].cast<12,2>()
				- x
				;
		}
	}
	//cerr << "1,final:" << count << endl;
	hash_wdo.clear();
	return sum;
}