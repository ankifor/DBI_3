#pragma once
#include "Types.hpp"
#include <tuple>
#include <vector>
#include <unordered_map>
#include <map>
#include <fstream>

using namespace std;

struct Table_warehouse {
	vector<Integer> w_id; //primary
	vector<Varchar<10>> w_name;
	vector<Varchar<20>> w_street_1;
	vector<Varchar<20>> w_street_2;
	vector<Varchar<20>> w_city;
	vector<Char<2>> w_state;
	vector<Char<9>> w_zip;
	vector<Numeric<4,4>> w_tax;
	vector<Numeric<12,2>> w_ytd;

	// primary_key: w_id
	using type_primary_key = tuple<Integer>;
	unordered_map<type_primary_key,Tid,hash_types::hash<type_primary_key>> primary_key;

	size_t size() {return w_id.size();}
	void read_from_file(ifstream& in);
	Tid insert(Integer in_w_id,Varchar<10> in_w_name,Varchar<20> in_w_street_1,Varchar<20> in_w_street_2,Varchar<20> in_w_city,Char<2> in_w_state,Char<9> in_w_zip,Numeric<4,4> in_w_tax,Numeric<12,2> in_w_ytd);
	void remove(Tid tid);
};

struct Table_district {
	vector<Integer> d_id; //primary
	vector<Integer> d_w_id; //primary
	vector<Varchar<10>> d_name;
	vector<Varchar<20>> d_street_1;
	vector<Varchar<20>> d_street_2;
	vector<Varchar<20>> d_city;
	vector<Char<2>> d_state;
	vector<Char<9>> d_zip;
	vector<Numeric<4,4>> d_tax;
	vector<Numeric<12,2>> d_ytd;
	vector<Integer> d_next_o_id;

	// primary_key: d_w_id, d_id
	using type_primary_key = tuple<Integer,Integer>;
	unordered_map<type_primary_key,Tid,hash_types::hash<type_primary_key>> primary_key;

	size_t size() {return d_id.size();}
	void read_from_file(ifstream& in);
	Tid insert(Integer in_d_id,Integer in_d_w_id,Varchar<10> in_d_name,Varchar<20> in_d_street_1,Varchar<20> in_d_street_2,Varchar<20> in_d_city,Char<2> in_d_state,Char<9> in_d_zip,Numeric<4,4> in_d_tax,Numeric<12,2> in_d_ytd,Integer in_d_next_o_id);
	void remove(Tid tid);
};

struct Table_customer {
	vector<Integer> c_id; //primary
	vector<Integer> c_d_id; //primary
	vector<Integer> c_w_id; //primary
	vector<Varchar<16>> c_first;
	vector<Char<2>> c_middle;
	vector<Varchar<16>> c_last;
	vector<Varchar<20>> c_street_1;
	vector<Varchar<20>> c_street_2;
	vector<Varchar<20>> c_city;
	vector<Char<2>> c_state;
	vector<Char<9>> c_zip;
	vector<Char<16>> c_phone;
	vector<Timestamp> c_since;
	vector<Char<2>> c_credit;
	vector<Numeric<12,2>> c_credit_lim;
	vector<Numeric<4,4>> c_discount;
	vector<Numeric<12,2>> c_balance;
	vector<Numeric<12,2>> c_ytd_paymenr;
	vector<Numeric<4,0>> c_payment_cnt;
	vector<Numeric<4,0>> c_delivery_cnt;
	vector<Varchar<500>> c_data;

	// primary_key: c_w_id, c_d_id, c_id
	using type_primary_key = tuple<Integer,Integer,Integer>;
	unordered_map<type_primary_key,Tid,hash_types::hash<type_primary_key>> primary_key;
	// customer_wdl: c_w_id, c_d_id, c_last, c_first
	using type_customer_wdl = tuple<Integer,Integer,Varchar<16>,Varchar<16>>;
	unordered_multimap<type_customer_wdl,Tid,hash_types::hash<type_customer_wdl>> customer_wdl;

	size_t size() {return c_id.size();}
	void read_from_file(ifstream& in);
	Tid insert(Integer in_c_id,Integer in_c_d_id,Integer in_c_w_id,Varchar<16> in_c_first,Char<2> in_c_middle,Varchar<16> in_c_last,Varchar<20> in_c_street_1,Varchar<20> in_c_street_2,Varchar<20> in_c_city,Char<2> in_c_state,Char<9> in_c_zip,Char<16> in_c_phone,Timestamp in_c_since,Char<2> in_c_credit,Numeric<12,2> in_c_credit_lim,Numeric<4,4> in_c_discount,Numeric<12,2> in_c_balance,Numeric<12,2> in_c_ytd_paymenr,Numeric<4,0> in_c_payment_cnt,Numeric<4,0> in_c_delivery_cnt,Varchar<500> in_c_data);
	void remove(Tid tid);
};

struct Table_history {
	vector<Integer> h_c_id;
	vector<Integer> h_c_d_id;
	vector<Integer> h_c_w_id;
	vector<Integer> h_d_id;
	vector<Integer> h_w_id;
	vector<Timestamp> h_date;
	vector<Numeric<6,2>> h_amount;
	vector<Varchar<24>> h_data;


	size_t size() {return h_c_id.size();}
	void read_from_file(ifstream& in);
	Tid insert(Integer in_h_c_id,Integer in_h_c_d_id,Integer in_h_c_w_id,Integer in_h_d_id,Integer in_h_w_id,Timestamp in_h_date,Numeric<6,2> in_h_amount,Varchar<24> in_h_data);
	void remove(Tid tid);
};

struct Table_neworder {
	vector<Integer> no_o_id; //primary
	vector<Integer> no_d_id; //primary
	vector<Integer> no_w_id; //primary

	// primary_key: no_w_id, no_d_id, no_o_id
	using type_primary_key = tuple<Integer,Integer,Integer>;
	map<type_primary_key,Tid> primary_key;

	size_t size() {return no_o_id.size();}
	void read_from_file(ifstream& in);
	Tid insert(Integer in_no_o_id,Integer in_no_d_id,Integer in_no_w_id);
	void remove(Tid tid);
};

struct Table_order {
	vector<Integer> o_id; //primary
	vector<Integer> o_d_id; //primary
	vector<Integer> o_w_id; //primary
	vector<Integer> o_c_id;
	vector<Timestamp> o_entry_d;
	vector<Integer> o_carrier_id;
	vector<Numeric<2,0>> o_ol_cnt;
	vector<Numeric<1,0>> o_all_local;

	// primary_key: o_w_id, o_d_id, o_id
	using type_primary_key = tuple<Integer,Integer,Integer>;
	unordered_map<type_primary_key,Tid,hash_types::hash<type_primary_key>> primary_key;
	// order_wdc: o_w_id, o_d_id, o_c_id, o_id
	using type_order_wdc = tuple<Integer,Integer,Integer,Integer>;
	unordered_multimap<type_order_wdc,Tid,hash_types::hash<type_order_wdc>> order_wdc;

	size_t size() {return o_id.size();}
	void read_from_file(ifstream& in);
	Tid insert(Integer in_o_id,Integer in_o_d_id,Integer in_o_w_id,Integer in_o_c_id,Timestamp in_o_entry_d,Integer in_o_carrier_id,Numeric<2,0> in_o_ol_cnt,Numeric<1,0> in_o_all_local);
	void remove(Tid tid);
};

struct Table_orderline {
	vector<Integer> ol_o_id; //primary
	vector<Integer> ol_d_id; //primary
	vector<Integer> ol_w_id; //primary
	vector<Integer> ol_number; //primary
	vector<Integer> ol_i_id;
	vector<Integer> ol_supply_w_id;
	vector<Timestamp> ol_delivery_d;
	vector<Numeric<2,0>> ol_quantity;
	vector<Numeric<6,2>> ol_amount;
	vector<Char<24>> ol_dist_info;

	// primary_key: ol_w_id, ol_d_id, ol_o_id, ol_number
	using type_primary_key = tuple<Integer,Integer,Integer,Integer>;
	unordered_map<type_primary_key,Tid,hash_types::hash<type_primary_key>> primary_key;

	size_t size() {return ol_o_id.size();}
	void read_from_file(ifstream& in);
	Tid insert(Integer in_ol_o_id,Integer in_ol_d_id,Integer in_ol_w_id,Integer in_ol_number,Integer in_ol_i_id,Integer in_ol_supply_w_id,Timestamp in_ol_delivery_d,Numeric<2,0> in_ol_quantity,Numeric<6,2> in_ol_amount,Char<24> in_ol_dist_info);
	void remove(Tid tid);
};

struct Table_item {
	vector<Integer> i_id; //primary
	vector<Integer> i_im_id;
	vector<Varchar<24>> i_name;
	vector<Numeric<5,2>> i_price;
	vector<Varchar<50>> i_data;

	// primary_key: i_id
	using type_primary_key = tuple<Integer>;
	unordered_map<type_primary_key,Tid,hash_types::hash<type_primary_key>> primary_key;

	size_t size() {return i_id.size();}
	void read_from_file(ifstream& in);
	Tid insert(Integer in_i_id,Integer in_i_im_id,Varchar<24> in_i_name,Numeric<5,2> in_i_price,Varchar<50> in_i_data);
	void remove(Tid tid);
};

struct Table_stock {
	vector<Integer> s_i_id; //primary
	vector<Integer> s_w_id; //primary
	vector<Numeric<4,0>> s_quantity;
	vector<Char<24>> s_dist_01;
	vector<Char<24>> s_dist_02;
	vector<Char<24>> s_dist_03;
	vector<Char<24>> s_dist_04;
	vector<Char<24>> s_dist_05;
	vector<Char<24>> s_dist_06;
	vector<Char<24>> s_dist_07;
	vector<Char<24>> s_dist_08;
	vector<Char<24>> s_dist_09;
	vector<Char<24>> s_dist_10;
	vector<Numeric<8,0>> s_ytd;
	vector<Numeric<4,0>> s_order_cnt;
	vector<Numeric<4,0>> s_remote_cnt;
	vector<Varchar<50>> s_data;

	// primary_key: s_w_id, s_i_id
	using type_primary_key = tuple<Integer,Integer>;
	unordered_map<type_primary_key,Tid,hash_types::hash<type_primary_key>> primary_key;

	size_t size() {return s_i_id.size();}
	void read_from_file(ifstream& in);
	Tid insert(Integer in_s_i_id,Integer in_s_w_id,Numeric<4,0> in_s_quantity,Char<24> in_s_dist_01,Char<24> in_s_dist_02,Char<24> in_s_dist_03,Char<24> in_s_dist_04,Char<24> in_s_dist_05,Char<24> in_s_dist_06,Char<24> in_s_dist_07,Char<24> in_s_dist_08,Char<24> in_s_dist_09,Char<24> in_s_dist_10,Numeric<8,0> in_s_ytd,Numeric<4,0> in_s_order_cnt,Numeric<4,0> in_s_remote_cnt,Varchar<50> in_s_data);
	void remove(Tid tid);
};


extern Table_warehouse warehouse;
extern Table_district district;
extern Table_customer customer;
extern Table_history history;
extern Table_neworder neworder;
extern Table_order order;
extern Table_orderline orderline;
extern Table_item item;
extern Table_stock stock;

