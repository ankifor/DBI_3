#include <unordered_map>

#include "Types.hpp"
#include "schema_1.hpp"
#include "Help.hpp"

#include <iostream>

using namespace std;

Numeric<12,2> join_query() {
	size_t n_buckets_wdc = 10000;
	size_t n_buckets_wdo = 10000;
	
	using type_wdc = tuple<Integer,Integer,Integer>;//w_id,d_id,c_id
	unordered_map<type_wdc,Tid,hash_types::hash<type_wdc>> customer_wdc;
	customer_wdc.rehash(n_buckets_wdc);
	// filter and create hash table
	for (Tid tid = 0; tid < customer.size(); ++tid) {
		if (customer.c_last[tid].len >= 1 && customer.c_last[tid].value[0] == 'B') {
			auto t = make_tuple(customer.c_w_id[tid], customer.c_d_id[tid], customer.c_id[tid]);
			customer_wdc.insert(make_pair(t,tid));
		}
	}
	
	//cout << customer_wdc.size() << endl;
	// match order with customer_wdc and create new hash, storing pairs of TID
	using type_wdo = tuple<Integer,Integer,Integer>;//w_id,d_id,o_id
	unordered_map<type_wdo,tuple<Tid, Tid>,hash_types::hash<type_wdo>> customer_order_wdo;
	customer_order_wdo.rehash(n_buckets_wdo);
	
	for (Tid tid = 0; tid < order.size(); ++tid) {
		auto t = make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_c_id[tid]);
		auto it = customer_wdc.find(t);
		if (it != customer_wdc.end()) {
			auto t_new = make_tuple(order.o_w_id[tid], order.o_d_id[tid], order.o_id[tid]);
			auto tids = make_tuple(it->second, tid);
			customer_order_wdo.insert(make_pair(t_new, tids));
		}
	}
	
	//cout << customer_order_wdo.size() << endl;
	// match orderline with customer_order_wdo and calculate sum
	Numeric<12,2> sum = 0;
	for (Tid tid = 0; tid < orderline.size(); ++tid) {
		auto t = make_tuple(orderline.ol_w_id[tid],orderline.ol_d_id[tid], orderline.ol_o_id[tid]);
		auto it = customer_order_wdo.find(t);
		if (it != customer_order_wdo.end()) {
			Tid tid_c = get<0>(it->second);
			Tid tid_o = get<1>(it->second);
			Tid tid_ol = tid;
			//select sum(ol_quantity*ol_amount-c_balance*o_ol_cnt)
			
			sum 
				+= (orderline.ol_quantity[tid_ol].cast<12,2>() * orderline.ol_amount[tid_ol].cast<12,2>()
				- customer.c_balance[tid_c] * order.o_ol_cnt[tid_o].cast<12,2>()).cast<12,2>()
				;
		}
	}
	return sum;


}