#include <unordered_map>

#include "Types.hpp"
#include "schema_1.hpp"
#include "Help.hpp"

#include <iostream>

using namespace std;

Numeric<12,4> join_query() {
	size_t n_buckets_wdc = 10000;
	size_t n_buckets_wdo = 10000;
	
	using type_key_wdc = tuple<Integer,Integer,Integer>;//w_id,d_id,c_id
	using type_val_wdc = tuple<Numeric<12,2>>;//c_balance
	unordered_map<type_key_wdc,type_val_wdc,hash_types::hash<type_key_wdc>> hash_wdc;
	hash_wdc.rehash(n_buckets_wdc);
	// filter and create hash table
	for (Tid tid = 0; tid < customer.size(); ++tid) {
		if (customer.c_last[tid].len >= 1 && customer.c_last[tid].value[0] == 'B') {
			auto t_key = make_tuple(customer.c_w_id[tid], customer.c_d_id[tid], customer.c_id[tid]);
			auto t_val = make_tuple(customer.c_balance[tid]);
			hash_wdc.insert(make_pair(t_key,t_val));
		}
	}
	
	//cout << customer_wdc.size() << endl;
	// match order with customer_wdc and create new hash, storing pairs of TID
	using type_key_wdo = tuple<Integer,Integer,Integer>;//w_id,d_id,o_id
	using type_val_wdo = tuple<Numeric<12,4>>;//c_balance * o_ol_cnt
	unordered_map<type_key_wdo,type_val_wdo,hash_types::hash<type_key_wdo>> hash_wdo;
	
	hash_wdo.rehash(n_buckets_wdo);
	
	for (Tid tid = 0; tid < order.size(); ++tid) {
		auto t_key = make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_c_id[tid]);
		auto it = hash_wdc.find(t_key);
		if (it != hash_wdc.end()) {
			auto t_new = make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_id[tid]);
			//auto t_val = tuple_cat(it->second, make_tuple(order.o_ol_cnt[tid]));
			auto t_val = make_tuple(get<0>(it->second) * order.o_ol_cnt[tid].cast<12,2>());
			hash_wdo.insert(make_pair(t_new, t_val));
		}
	}
	
	//cout << customer_order_wdo.size() << endl;
	// match orderline with customer_order_wdo and calculate sum
	Numeric<12,4> sum = 0;
	for (Tid tid = 0; tid < orderline.size(); ++tid) {
		auto t_key = make_tuple(orderline.ol_w_id[tid],orderline.ol_d_id[tid], orderline.ol_o_id[tid]);
		auto it = hash_wdo.find(t_key);
		if (it != hash_wdo.end()) {
			auto x = get<0>(it->second);
			//select sum(ol_quantity*ol_amount-c_balance*o_ol_cnt)
			
			sum 
				+= orderline.ol_quantity[tid].cast<12,2>() * orderline.ol_amount[tid].cast<12,2>()
				- x
				;
		}
	}
	return sum;


}



void join_query1() {
	size_t n_buckets_wdc = 10000;
	size_t n_buckets_wdo = 10000;
	
	using type_key_wdc = tuple<Integer,Integer,Integer>;//w_id,d_id,c_id
	using type_val_wdc = tuple<Varchar<16>,Varchar<16>>;//c_first,c_last
	unordered_map<type_key_wdc,type_val_wdc,hash_types::hash<type_key_wdc>> hash_wdc;
	hash_wdc.rehash(n_buckets_wdc);

	for (Tid tid = 0; tid < customer.size(); ++tid) {
		if (customer.c_last[tid].len >= 1 && customer.c_last[tid].value[0] == 'B') {
			auto t_key = make_tuple(customer.c_w_id[tid], customer.c_d_id[tid], customer.c_id[tid]);
			auto t_val = make_tuple(customer.c_first[tid],customer.c_last[tid]);
			hash_wdc.insert(make_pair(t_key,t_val));
		}
	}
	
	using type_key_wdo = tuple<Integer,Integer,Integer>;//w_id,d_id,o_id
	using type_val_wdo = tuple<Varchar<16>,Varchar<16>,Numeric<1,0>>;//c_first,c_last,o_all_local
	unordered_map<type_key_wdo,type_val_wdo,hash_types::hash<type_key_wdo>> hash_wdo;
	
	hash_wdo.rehash(n_buckets_wdo);
	
	for (Tid tid = 0; tid < order.size(); ++tid) {
		auto t_key = make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_c_id[tid]);
		auto it = hash_wdc.find(t_key);
		if (it != hash_wdc.end()) {
			auto t_new = make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_id[tid]);
			//auto t_val = tuple_cat(it->second, make_tuple(order.o_ol_cnt[tid]));
			auto t_val = make_tuple(get<0>(it->second),get<1>(it->second),order.o_all_local[tid]);
			hash_wdo.insert(make_pair(t_new, t_val));
		}
	}
	
	for (Tid tid = 0; tid < orderline.size(); ++tid) {
		auto t_key = make_tuple(orderline.ol_w_id[tid],orderline.ol_d_id[tid], orderline.ol_o_id[tid]);
		auto it = hash_wdo.find(t_key);
		if (it != hash_wdo.end()) {
			//c_first, c_last, o_all_local, ol_amount
			cout << get<0>(it->second)       << ","
				 << get<1>(it->second)       << ","
				 << get<2>(it->second)       << ","
				 << orderline.ol_amount[tid] << endl;
		}
	}
}