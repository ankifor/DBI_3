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
	
	using type_key1 = tuple<Integer,Integer,Integer>;//w_id,d_id,c_id
	using type_val1 = tuple<Varchar<16>,Varchar<16>>;//c_first,c_last
	unordered_map<type_key1,type_val1,hash_types::hash<type_key1>> hash1;
	hash1.rehash(n_buckets_wdc);

	for (Tid tid1 = 0; tid1 < customer.size(); ++tid1) {
		if (customer.c_last[tid1].len >= 1 && customer.c_last[tid1].value[0] == 'B') {
			auto t_key1 = make_tuple(customer.c_w_id[tid1], customer.c_d_id[tid1], customer.c_id[tid1]);
			auto t_val1 = make_tuple(customer.c_first[tid1],customer.c_last[tid1]);
			hash1.insert(make_pair(t_key1,t_val1));
		}
	}
	
	using type_key2 = tuple<Integer,Integer,Integer>;//w_id,d_id,o_id
	using type_val2 = tuple<Varchar<16>,Varchar<16>,Numeric<1,0>>;//c_first,c_last,o_all_local
	unordered_map<type_key2,type_val2,hash_types::hash<type_key2>> hash2;
	
	hash2.rehash(n_buckets_wdo);
	
	for (Tid tid2 = 0; tid2 < order.size(); ++tid2) {
		auto t_key2 = make_tuple(order.o_w_id[tid2], order.o_d_id[tid2], order.o_c_id[tid2]);
		auto it1 = hash1.find(t_key2);
		if (it1 != hash1.end()) {
			auto t_key3 = make_tuple(order.o_w_id[tid2], order.o_d_id[tid2], order.o_id[tid2]);
			//auto t_val = tuple_cat(it->second, make_tuple(order.o_ol_cnt[tid]));
			auto t_val2 = make_tuple(get<0>(it1->second),get<1>(it1->second),order.o_all_local[tid2]);
			hash2.insert(make_pair(t_key3, t_val2));
		}
	}
	
	for (Tid tid3 = 0; tid3 < orderline.size(); ++tid3) {
		auto t_key4 = make_tuple(orderline.ol_w_id[tid3],orderline.ol_d_id[tid3], orderline.ol_o_id[tid3]);
		auto it2 = hash2.find(t_key4);
		if (it2 != hash2.end()) {
			//c_first, c_last, o_all_local, ol_amount
			cout << get<0>(it2->second)       << ","
				 << get<1>(it2->second)       << ","
				 << get<2>(it2->second)       << ","
				 << orderline.ol_amount[tid3] << endl;
		}
	}
}