#include "schema_1.hpp"
#include "Types.hpp"
#include "Help.hpp"
#include <cassert>
#include <string>
#include <utility>

using namespace std;

static const char FLD_DLM = '|';
static const char ROW_DLM = '\n';

Tid Table_warehouse::insert(Integer in_w_id,Varchar<10> in_w_name,Varchar<20> in_w_street_1,Varchar<20> in_w_street_2,Varchar<20> in_w_city,Char<2> in_w_state,Char<9> in_w_zip,Numeric<4,4> in_w_tax,Numeric<12,2> in_w_ytd)
{
	Tid new_tid = size();
	check_key(primary_key, in_w_id);
	insert_key<false>(primary_key, new_tid, in_w_id);
	w_id.push_back(in_w_id);
	w_name.push_back(in_w_name);
	w_street_1.push_back(in_w_street_1);
	w_street_2.push_back(in_w_street_2);
	w_city.push_back(in_w_city);
	w_state.push_back(in_w_state);
	w_zip.push_back(in_w_zip);
	w_tax.push_back(in_w_tax);
	w_ytd.push_back(in_w_ytd);
	return new_tid;
}

void Table_warehouse::remove(Tid tid)
{
	Tid last_tid = size() - 1;
	assert(tid <= last_tid);
	remove_key(primary_key,tid,w_id[tid]);
	if (tid != last_tid) {
		replace_key(primary_key,last_tid,tid,w_id[last_tid]);
		w_id[tid] = w_id[last_tid];
		w_name[tid] = w_name[last_tid];
		w_street_1[tid] = w_street_1[last_tid];
		w_street_2[tid] = w_street_2[last_tid];
		w_city[tid] = w_city[last_tid];
		w_state[tid] = w_state[last_tid];
		w_zip[tid] = w_zip[last_tid];
		w_tax[tid] = w_tax[last_tid];
		w_ytd[tid] = w_ytd[last_tid];
	}
	w_id.pop_back();
	w_name.pop_back();
	w_street_1.pop_back();
	w_street_2.pop_back();
	w_city.pop_back();
	w_state.pop_back();
	w_zip.pop_back();
	w_tax.pop_back();
	w_ytd.pop_back();
}

void Table_warehouse::read_from_file(ifstream& in)
{
	assert(in.is_open());
	string buf_field; buf_field.reserve(500);
	Integer in_w_id;
	Varchar<10> in_w_name;
	Varchar<20> in_w_street_1;
	Varchar<20> in_w_street_2;
	Varchar<20> in_w_city;
	Char<2> in_w_state;
	Char<9> in_w_zip;
	Numeric<4,4> in_w_tax;
	Numeric<12,2> in_w_ytd;
	char first_symbol = 0;
	while (!in.eof()) {
		first_symbol = in.get();
		if (first_symbol == ROW_DLM || first_symbol == EOF) continue;
		in.unget();
		assert(!in.fail());

		getline(in, buf_field, FLD_DLM); in_w_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_w_name = Varchar<10>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_w_street_1 = Varchar<20>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_w_street_2 = Varchar<20>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_w_city = Varchar<20>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_w_state = Char<2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_w_zip = Char<9>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_w_tax = Numeric<4,4>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, ROW_DLM); in_w_ytd = Numeric<12,2>::castString(buf_field.data(), buf_field.length());

		insert(in_w_id,in_w_name,in_w_street_1,in_w_street_2,in_w_city,in_w_state,in_w_zip,in_w_tax,in_w_ytd);
	}
}


Tid Table_district::insert(Integer in_d_id,Integer in_d_w_id,Varchar<10> in_d_name,Varchar<20> in_d_street_1,Varchar<20> in_d_street_2,Varchar<20> in_d_city,Char<2> in_d_state,Char<9> in_d_zip,Numeric<4,4> in_d_tax,Numeric<12,2> in_d_ytd,Integer in_d_next_o_id)
{
	Tid new_tid = size();
	check_key(primary_key, in_d_w_id, in_d_id);
	insert_key<false>(primary_key, new_tid, in_d_w_id, in_d_id);
	d_id.push_back(in_d_id);
	d_w_id.push_back(in_d_w_id);
	d_name.push_back(in_d_name);
	d_street_1.push_back(in_d_street_1);
	d_street_2.push_back(in_d_street_2);
	d_city.push_back(in_d_city);
	d_state.push_back(in_d_state);
	d_zip.push_back(in_d_zip);
	d_tax.push_back(in_d_tax);
	d_ytd.push_back(in_d_ytd);
	d_next_o_id.push_back(in_d_next_o_id);
	return new_tid;
}

void Table_district::remove(Tid tid)
{
	Tid last_tid = size() - 1;
	assert(tid <= last_tid);
	remove_key(primary_key,tid,d_w_id[tid],d_id[tid]);
	if (tid != last_tid) {
		replace_key(primary_key,last_tid,tid,d_w_id[last_tid],d_id[last_tid]);
		d_id[tid] = d_id[last_tid];
		d_w_id[tid] = d_w_id[last_tid];
		d_name[tid] = d_name[last_tid];
		d_street_1[tid] = d_street_1[last_tid];
		d_street_2[tid] = d_street_2[last_tid];
		d_city[tid] = d_city[last_tid];
		d_state[tid] = d_state[last_tid];
		d_zip[tid] = d_zip[last_tid];
		d_tax[tid] = d_tax[last_tid];
		d_ytd[tid] = d_ytd[last_tid];
		d_next_o_id[tid] = d_next_o_id[last_tid];
	}
	d_id.pop_back();
	d_w_id.pop_back();
	d_name.pop_back();
	d_street_1.pop_back();
	d_street_2.pop_back();
	d_city.pop_back();
	d_state.pop_back();
	d_zip.pop_back();
	d_tax.pop_back();
	d_ytd.pop_back();
	d_next_o_id.pop_back();
}

void Table_district::read_from_file(ifstream& in)
{
	assert(in.is_open());
	string buf_field; buf_field.reserve(500);
	Integer in_d_id;
	Integer in_d_w_id;
	Varchar<10> in_d_name;
	Varchar<20> in_d_street_1;
	Varchar<20> in_d_street_2;
	Varchar<20> in_d_city;
	Char<2> in_d_state;
	Char<9> in_d_zip;
	Numeric<4,4> in_d_tax;
	Numeric<12,2> in_d_ytd;
	Integer in_d_next_o_id;
	char first_symbol = 0;
	while (!in.eof()) {
		first_symbol = in.get();
		if (first_symbol == ROW_DLM || first_symbol == EOF) continue;
		in.unget();
		assert(!in.fail());

		getline(in, buf_field, FLD_DLM); in_d_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_d_w_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_d_name = Varchar<10>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_d_street_1 = Varchar<20>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_d_street_2 = Varchar<20>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_d_city = Varchar<20>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_d_state = Char<2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_d_zip = Char<9>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_d_tax = Numeric<4,4>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_d_ytd = Numeric<12,2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, ROW_DLM); in_d_next_o_id = Integer::castString(buf_field.data(), buf_field.length());

		insert(in_d_id,in_d_w_id,in_d_name,in_d_street_1,in_d_street_2,in_d_city,in_d_state,in_d_zip,in_d_tax,in_d_ytd,in_d_next_o_id);
	}
}


Tid Table_customer::insert(Integer in_c_id,Integer in_c_d_id,Integer in_c_w_id,Varchar<16> in_c_first,Char<2> in_c_middle,Varchar<16> in_c_last,Varchar<20> in_c_street_1,Varchar<20> in_c_street_2,Varchar<20> in_c_city,Char<2> in_c_state,Char<9> in_c_zip,Char<16> in_c_phone,Timestamp in_c_since,Char<2> in_c_credit,Numeric<12,2> in_c_credit_lim,Numeric<4,4> in_c_discount,Numeric<12,2> in_c_balance,Numeric<12,2> in_c_ytd_paymenr,Numeric<4,0> in_c_payment_cnt,Numeric<4,0> in_c_delivery_cnt,Varchar<500> in_c_data)
{
	Tid new_tid = size();
	check_key(primary_key, in_c_w_id, in_c_d_id, in_c_id);
	insert_key<false>(primary_key, new_tid, in_c_w_id, in_c_d_id, in_c_id);
	insert_key<false>(customer_wdl, new_tid, in_c_w_id, in_c_d_id, in_c_last, in_c_first);
	c_id.push_back(in_c_id);
	c_d_id.push_back(in_c_d_id);
	c_w_id.push_back(in_c_w_id);
	c_first.push_back(in_c_first);
	c_middle.push_back(in_c_middle);
	c_last.push_back(in_c_last);
	c_street_1.push_back(in_c_street_1);
	c_street_2.push_back(in_c_street_2);
	c_city.push_back(in_c_city);
	c_state.push_back(in_c_state);
	c_zip.push_back(in_c_zip);
	c_phone.push_back(in_c_phone);
	c_since.push_back(in_c_since);
	c_credit.push_back(in_c_credit);
	c_credit_lim.push_back(in_c_credit_lim);
	c_discount.push_back(in_c_discount);
	c_balance.push_back(in_c_balance);
	c_ytd_paymenr.push_back(in_c_ytd_paymenr);
	c_payment_cnt.push_back(in_c_payment_cnt);
	c_delivery_cnt.push_back(in_c_delivery_cnt);
	c_data.push_back(in_c_data);
	return new_tid;
}

void Table_customer::remove(Tid tid)
{
	Tid last_tid = size() - 1;
	assert(tid <= last_tid);
	remove_key(primary_key,tid,c_w_id[tid],c_d_id[tid],c_id[tid]);
	remove_key(customer_wdl,tid,c_w_id[tid],c_d_id[tid],c_last[tid],c_first[tid]);
	if (tid != last_tid) {
		replace_key(primary_key,last_tid,tid,c_w_id[last_tid],c_d_id[last_tid],c_id[last_tid]);
		replace_key(customer_wdl,last_tid,tid,c_w_id[last_tid],c_d_id[last_tid],c_last[last_tid],c_first[last_tid]);
		c_id[tid] = c_id[last_tid];
		c_d_id[tid] = c_d_id[last_tid];
		c_w_id[tid] = c_w_id[last_tid];
		c_first[tid] = c_first[last_tid];
		c_middle[tid] = c_middle[last_tid];
		c_last[tid] = c_last[last_tid];
		c_street_1[tid] = c_street_1[last_tid];
		c_street_2[tid] = c_street_2[last_tid];
		c_city[tid] = c_city[last_tid];
		c_state[tid] = c_state[last_tid];
		c_zip[tid] = c_zip[last_tid];
		c_phone[tid] = c_phone[last_tid];
		c_since[tid] = c_since[last_tid];
		c_credit[tid] = c_credit[last_tid];
		c_credit_lim[tid] = c_credit_lim[last_tid];
		c_discount[tid] = c_discount[last_tid];
		c_balance[tid] = c_balance[last_tid];
		c_ytd_paymenr[tid] = c_ytd_paymenr[last_tid];
		c_payment_cnt[tid] = c_payment_cnt[last_tid];
		c_delivery_cnt[tid] = c_delivery_cnt[last_tid];
		c_data[tid] = c_data[last_tid];
	}
	c_id.pop_back();
	c_d_id.pop_back();
	c_w_id.pop_back();
	c_first.pop_back();
	c_middle.pop_back();
	c_last.pop_back();
	c_street_1.pop_back();
	c_street_2.pop_back();
	c_city.pop_back();
	c_state.pop_back();
	c_zip.pop_back();
	c_phone.pop_back();
	c_since.pop_back();
	c_credit.pop_back();
	c_credit_lim.pop_back();
	c_discount.pop_back();
	c_balance.pop_back();
	c_ytd_paymenr.pop_back();
	c_payment_cnt.pop_back();
	c_delivery_cnt.pop_back();
	c_data.pop_back();
}

void Table_customer::read_from_file(ifstream& in)
{
	assert(in.is_open());
	string buf_field; buf_field.reserve(500);
	Integer in_c_id;
	Integer in_c_d_id;
	Integer in_c_w_id;
	Varchar<16> in_c_first;
	Char<2> in_c_middle;
	Varchar<16> in_c_last;
	Varchar<20> in_c_street_1;
	Varchar<20> in_c_street_2;
	Varchar<20> in_c_city;
	Char<2> in_c_state;
	Char<9> in_c_zip;
	Char<16> in_c_phone;
	Timestamp in_c_since;
	Char<2> in_c_credit;
	Numeric<12,2> in_c_credit_lim;
	Numeric<4,4> in_c_discount;
	Numeric<12,2> in_c_balance;
	Numeric<12,2> in_c_ytd_paymenr;
	Numeric<4,0> in_c_payment_cnt;
	Numeric<4,0> in_c_delivery_cnt;
	Varchar<500> in_c_data;
	char first_symbol = 0;
	while (!in.eof()) {
		first_symbol = in.get();
		if (first_symbol == ROW_DLM || first_symbol == EOF) continue;
		in.unget();
		assert(!in.fail());

		getline(in, buf_field, FLD_DLM); in_c_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_d_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_w_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_first = Varchar<16>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_middle = Char<2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_last = Varchar<16>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_street_1 = Varchar<20>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_street_2 = Varchar<20>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_city = Varchar<20>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_state = Char<2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_zip = Char<9>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_phone = Char<16>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_since = Timestamp::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_credit = Char<2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_credit_lim = Numeric<12,2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_discount = Numeric<4,4>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_balance = Numeric<12,2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_ytd_paymenr = Numeric<12,2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_payment_cnt = Numeric<4,0>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_c_delivery_cnt = Numeric<4,0>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, ROW_DLM); in_c_data = Varchar<500>::castString(buf_field.data(), buf_field.length());

		insert(in_c_id,in_c_d_id,in_c_w_id,in_c_first,in_c_middle,in_c_last,in_c_street_1,in_c_street_2,in_c_city,in_c_state,in_c_zip,in_c_phone,in_c_since,in_c_credit,in_c_credit_lim,in_c_discount,in_c_balance,in_c_ytd_paymenr,in_c_payment_cnt,in_c_delivery_cnt,in_c_data);
	}
}


Tid Table_history::insert(Integer in_h_c_id,Integer in_h_c_d_id,Integer in_h_c_w_id,Integer in_h_d_id,Integer in_h_w_id,Timestamp in_h_date,Numeric<6,2> in_h_amount,Varchar<24> in_h_data)
{
	Tid new_tid = size();
	h_c_id.push_back(in_h_c_id);
	h_c_d_id.push_back(in_h_c_d_id);
	h_c_w_id.push_back(in_h_c_w_id);
	h_d_id.push_back(in_h_d_id);
	h_w_id.push_back(in_h_w_id);
	h_date.push_back(in_h_date);
	h_amount.push_back(in_h_amount);
	h_data.push_back(in_h_data);
	return new_tid;
}

void Table_history::remove(Tid tid)
{
	Tid last_tid = size() - 1;
	assert(tid <= last_tid);
	if (tid != last_tid) {
		h_c_id[tid] = h_c_id[last_tid];
		h_c_d_id[tid] = h_c_d_id[last_tid];
		h_c_w_id[tid] = h_c_w_id[last_tid];
		h_d_id[tid] = h_d_id[last_tid];
		h_w_id[tid] = h_w_id[last_tid];
		h_date[tid] = h_date[last_tid];
		h_amount[tid] = h_amount[last_tid];
		h_data[tid] = h_data[last_tid];
	}
	h_c_id.pop_back();
	h_c_d_id.pop_back();
	h_c_w_id.pop_back();
	h_d_id.pop_back();
	h_w_id.pop_back();
	h_date.pop_back();
	h_amount.pop_back();
	h_data.pop_back();
}

void Table_history::read_from_file(ifstream& in)
{
	assert(in.is_open());
	string buf_field; buf_field.reserve(500);
	Integer in_h_c_id;
	Integer in_h_c_d_id;
	Integer in_h_c_w_id;
	Integer in_h_d_id;
	Integer in_h_w_id;
	Timestamp in_h_date;
	Numeric<6,2> in_h_amount;
	Varchar<24> in_h_data;
	char first_symbol = 0;
	while (!in.eof()) {
		first_symbol = in.get();
		if (first_symbol == ROW_DLM || first_symbol == EOF) continue;
		in.unget();
		assert(!in.fail());

		getline(in, buf_field, FLD_DLM); in_h_c_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_h_c_d_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_h_c_w_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_h_d_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_h_w_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_h_date = Timestamp::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_h_amount = Numeric<6,2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, ROW_DLM); in_h_data = Varchar<24>::castString(buf_field.data(), buf_field.length());

		insert(in_h_c_id,in_h_c_d_id,in_h_c_w_id,in_h_d_id,in_h_w_id,in_h_date,in_h_amount,in_h_data);
	}
}


Tid Table_neworder::insert(Integer in_no_o_id,Integer in_no_d_id,Integer in_no_w_id)
{
	Tid new_tid = size();
	check_key(primary_key, in_no_w_id, in_no_d_id, in_no_o_id);
	insert_key<false>(primary_key, new_tid, in_no_w_id, in_no_d_id, in_no_o_id);
	no_o_id.push_back(in_no_o_id);
	no_d_id.push_back(in_no_d_id);
	no_w_id.push_back(in_no_w_id);
	return new_tid;
}

void Table_neworder::remove(Tid tid)
{
	Tid last_tid = size() - 1;
	assert(tid <= last_tid);
	remove_key(primary_key,tid,no_w_id[tid],no_d_id[tid],no_o_id[tid]);
	if (tid != last_tid) {
		replace_key(primary_key,last_tid,tid,no_w_id[last_tid],no_d_id[last_tid],no_o_id[last_tid]);
		no_o_id[tid] = no_o_id[last_tid];
		no_d_id[tid] = no_d_id[last_tid];
		no_w_id[tid] = no_w_id[last_tid];
	}
	no_o_id.pop_back();
	no_d_id.pop_back();
	no_w_id.pop_back();
}

void Table_neworder::read_from_file(ifstream& in)
{
	assert(in.is_open());
	string buf_field; buf_field.reserve(500);
	Integer in_no_o_id;
	Integer in_no_d_id;
	Integer in_no_w_id;
	char first_symbol = 0;
	while (!in.eof()) {
		first_symbol = in.get();
		if (first_symbol == ROW_DLM || first_symbol == EOF) continue;
		in.unget();
		assert(!in.fail());

		getline(in, buf_field, FLD_DLM); in_no_o_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_no_d_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, ROW_DLM); in_no_w_id = Integer::castString(buf_field.data(), buf_field.length());

		insert(in_no_o_id,in_no_d_id,in_no_w_id);
	}
}


Tid Table_order::insert(Integer in_o_id,Integer in_o_d_id,Integer in_o_w_id,Integer in_o_c_id,Timestamp in_o_entry_d,Integer in_o_carrier_id,Numeric<2,0> in_o_ol_cnt,Numeric<1,0> in_o_all_local)
{
	Tid new_tid = size();
	check_key(primary_key, in_o_w_id, in_o_d_id, in_o_id);
	insert_key<false>(primary_key, new_tid, in_o_w_id, in_o_d_id, in_o_id);
	insert_key<false>(order_wdc, new_tid, in_o_w_id, in_o_d_id, in_o_c_id, in_o_id);
	o_id.push_back(in_o_id);
	o_d_id.push_back(in_o_d_id);
	o_w_id.push_back(in_o_w_id);
	o_c_id.push_back(in_o_c_id);
	o_entry_d.push_back(in_o_entry_d);
	o_carrier_id.push_back(in_o_carrier_id);
	o_ol_cnt.push_back(in_o_ol_cnt);
	o_all_local.push_back(in_o_all_local);
	return new_tid;
}

void Table_order::remove(Tid tid)
{
	Tid last_tid = size() - 1;
	assert(tid <= last_tid);
	remove_key(primary_key,tid,o_w_id[tid],o_d_id[tid],o_id[tid]);
	remove_key(order_wdc,tid,o_w_id[tid],o_d_id[tid],o_c_id[tid],o_id[tid]);
	if (tid != last_tid) {
		replace_key(primary_key,last_tid,tid,o_w_id[last_tid],o_d_id[last_tid],o_id[last_tid]);
		replace_key(order_wdc,last_tid,tid,o_w_id[last_tid],o_d_id[last_tid],o_c_id[last_tid],o_id[last_tid]);
		o_id[tid] = o_id[last_tid];
		o_d_id[tid] = o_d_id[last_tid];
		o_w_id[tid] = o_w_id[last_tid];
		o_c_id[tid] = o_c_id[last_tid];
		o_entry_d[tid] = o_entry_d[last_tid];
		o_carrier_id[tid] = o_carrier_id[last_tid];
		o_ol_cnt[tid] = o_ol_cnt[last_tid];
		o_all_local[tid] = o_all_local[last_tid];
	}
	o_id.pop_back();
	o_d_id.pop_back();
	o_w_id.pop_back();
	o_c_id.pop_back();
	o_entry_d.pop_back();
	o_carrier_id.pop_back();
	o_ol_cnt.pop_back();
	o_all_local.pop_back();
}

void Table_order::read_from_file(ifstream& in)
{
	assert(in.is_open());
	string buf_field; buf_field.reserve(500);
	Integer in_o_id;
	Integer in_o_d_id;
	Integer in_o_w_id;
	Integer in_o_c_id;
	Timestamp in_o_entry_d;
	Integer in_o_carrier_id;
	Numeric<2,0> in_o_ol_cnt;
	Numeric<1,0> in_o_all_local;
	char first_symbol = 0;
	while (!in.eof()) {
		first_symbol = in.get();
		if (first_symbol == ROW_DLM || first_symbol == EOF) continue;
		in.unget();
		assert(!in.fail());

		getline(in, buf_field, FLD_DLM); in_o_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_o_d_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_o_w_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_o_c_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_o_entry_d = Timestamp::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_o_carrier_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_o_ol_cnt = Numeric<2,0>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, ROW_DLM); in_o_all_local = Numeric<1,0>::castString(buf_field.data(), buf_field.length());

		insert(in_o_id,in_o_d_id,in_o_w_id,in_o_c_id,in_o_entry_d,in_o_carrier_id,in_o_ol_cnt,in_o_all_local);
	}
}


Tid Table_orderline::insert(Integer in_ol_o_id,Integer in_ol_d_id,Integer in_ol_w_id,Integer in_ol_number,Integer in_ol_i_id,Integer in_ol_supply_w_id,Timestamp in_ol_delivery_d,Numeric<2,0> in_ol_quantity,Numeric<6,2> in_ol_amount,Char<24> in_ol_dist_info)
{
	Tid new_tid = size();
	check_key(primary_key, in_ol_w_id, in_ol_d_id, in_ol_o_id, in_ol_number);
	insert_key<false>(primary_key, new_tid, in_ol_w_id, in_ol_d_id, in_ol_o_id, in_ol_number);
	ol_o_id.push_back(in_ol_o_id);
	ol_d_id.push_back(in_ol_d_id);
	ol_w_id.push_back(in_ol_w_id);
	ol_number.push_back(in_ol_number);
	ol_i_id.push_back(in_ol_i_id);
	ol_supply_w_id.push_back(in_ol_supply_w_id);
	ol_delivery_d.push_back(in_ol_delivery_d);
	ol_quantity.push_back(in_ol_quantity);
	ol_amount.push_back(in_ol_amount);
	ol_dist_info.push_back(in_ol_dist_info);
	return new_tid;
}

void Table_orderline::remove(Tid tid)
{
	Tid last_tid = size() - 1;
	assert(tid <= last_tid);
	remove_key(primary_key,tid,ol_w_id[tid],ol_d_id[tid],ol_o_id[tid],ol_number[tid]);
	if (tid != last_tid) {
		replace_key(primary_key,last_tid,tid,ol_w_id[last_tid],ol_d_id[last_tid],ol_o_id[last_tid],ol_number[last_tid]);
		ol_o_id[tid] = ol_o_id[last_tid];
		ol_d_id[tid] = ol_d_id[last_tid];
		ol_w_id[tid] = ol_w_id[last_tid];
		ol_number[tid] = ol_number[last_tid];
		ol_i_id[tid] = ol_i_id[last_tid];
		ol_supply_w_id[tid] = ol_supply_w_id[last_tid];
		ol_delivery_d[tid] = ol_delivery_d[last_tid];
		ol_quantity[tid] = ol_quantity[last_tid];
		ol_amount[tid] = ol_amount[last_tid];
		ol_dist_info[tid] = ol_dist_info[last_tid];
	}
	ol_o_id.pop_back();
	ol_d_id.pop_back();
	ol_w_id.pop_back();
	ol_number.pop_back();
	ol_i_id.pop_back();
	ol_supply_w_id.pop_back();
	ol_delivery_d.pop_back();
	ol_quantity.pop_back();
	ol_amount.pop_back();
	ol_dist_info.pop_back();
}

void Table_orderline::read_from_file(ifstream& in)
{
	assert(in.is_open());
	string buf_field; buf_field.reserve(500);
	Integer in_ol_o_id;
	Integer in_ol_d_id;
	Integer in_ol_w_id;
	Integer in_ol_number;
	Integer in_ol_i_id;
	Integer in_ol_supply_w_id;
	Timestamp in_ol_delivery_d;
	Numeric<2,0> in_ol_quantity;
	Numeric<6,2> in_ol_amount;
	Char<24> in_ol_dist_info;
	char first_symbol = 0;
	while (!in.eof()) {
		first_symbol = in.get();
		if (first_symbol == ROW_DLM || first_symbol == EOF) continue;
		in.unget();
		assert(!in.fail());

		getline(in, buf_field, FLD_DLM); in_ol_o_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_ol_d_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_ol_w_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_ol_number = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_ol_i_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_ol_supply_w_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_ol_delivery_d = Timestamp::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_ol_quantity = Numeric<2,0>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_ol_amount = Numeric<6,2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, ROW_DLM); in_ol_dist_info = Char<24>::castString(buf_field.data(), buf_field.length());

		insert(in_ol_o_id,in_ol_d_id,in_ol_w_id,in_ol_number,in_ol_i_id,in_ol_supply_w_id,in_ol_delivery_d,in_ol_quantity,in_ol_amount,in_ol_dist_info);
	}
}


Tid Table_item::insert(Integer in_i_id,Integer in_i_im_id,Varchar<24> in_i_name,Numeric<5,2> in_i_price,Varchar<50> in_i_data)
{
	Tid new_tid = size();
	check_key(primary_key, in_i_id);
	insert_key<false>(primary_key, new_tid, in_i_id);
	i_id.push_back(in_i_id);
	i_im_id.push_back(in_i_im_id);
	i_name.push_back(in_i_name);
	i_price.push_back(in_i_price);
	i_data.push_back(in_i_data);
	return new_tid;
}

void Table_item::remove(Tid tid)
{
	Tid last_tid = size() - 1;
	assert(tid <= last_tid);
	remove_key(primary_key,tid,i_id[tid]);
	if (tid != last_tid) {
		replace_key(primary_key,last_tid,tid,i_id[last_tid]);
		i_id[tid] = i_id[last_tid];
		i_im_id[tid] = i_im_id[last_tid];
		i_name[tid] = i_name[last_tid];
		i_price[tid] = i_price[last_tid];
		i_data[tid] = i_data[last_tid];
	}
	i_id.pop_back();
	i_im_id.pop_back();
	i_name.pop_back();
	i_price.pop_back();
	i_data.pop_back();
}

void Table_item::read_from_file(ifstream& in)
{
	assert(in.is_open());
	string buf_field; buf_field.reserve(500);
	Integer in_i_id;
	Integer in_i_im_id;
	Varchar<24> in_i_name;
	Numeric<5,2> in_i_price;
	Varchar<50> in_i_data;
	char first_symbol = 0;
	while (!in.eof()) {
		first_symbol = in.get();
		if (first_symbol == ROW_DLM || first_symbol == EOF) continue;
		in.unget();
		assert(!in.fail());

		getline(in, buf_field, FLD_DLM); in_i_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_i_im_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_i_name = Varchar<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_i_price = Numeric<5,2>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, ROW_DLM); in_i_data = Varchar<50>::castString(buf_field.data(), buf_field.length());

		insert(in_i_id,in_i_im_id,in_i_name,in_i_price,in_i_data);
	}
}


Tid Table_stock::insert(Integer in_s_i_id,Integer in_s_w_id,Numeric<4,0> in_s_quantity,Char<24> in_s_dist_01,Char<24> in_s_dist_02,Char<24> in_s_dist_03,Char<24> in_s_dist_04,Char<24> in_s_dist_05,Char<24> in_s_dist_06,Char<24> in_s_dist_07,Char<24> in_s_dist_08,Char<24> in_s_dist_09,Char<24> in_s_dist_10,Numeric<8,0> in_s_ytd,Numeric<4,0> in_s_order_cnt,Numeric<4,0> in_s_remote_cnt,Varchar<50> in_s_data)
{
	Tid new_tid = size();
	check_key(primary_key, in_s_w_id, in_s_i_id);
	insert_key<false>(primary_key, new_tid, in_s_w_id, in_s_i_id);
	s_i_id.push_back(in_s_i_id);
	s_w_id.push_back(in_s_w_id);
	s_quantity.push_back(in_s_quantity);
	s_dist_01.push_back(in_s_dist_01);
	s_dist_02.push_back(in_s_dist_02);
	s_dist_03.push_back(in_s_dist_03);
	s_dist_04.push_back(in_s_dist_04);
	s_dist_05.push_back(in_s_dist_05);
	s_dist_06.push_back(in_s_dist_06);
	s_dist_07.push_back(in_s_dist_07);
	s_dist_08.push_back(in_s_dist_08);
	s_dist_09.push_back(in_s_dist_09);
	s_dist_10.push_back(in_s_dist_10);
	s_ytd.push_back(in_s_ytd);
	s_order_cnt.push_back(in_s_order_cnt);
	s_remote_cnt.push_back(in_s_remote_cnt);
	s_data.push_back(in_s_data);
	return new_tid;
}

void Table_stock::remove(Tid tid)
{
	Tid last_tid = size() - 1;
	assert(tid <= last_tid);
	remove_key(primary_key,tid,s_w_id[tid],s_i_id[tid]);
	if (tid != last_tid) {
		replace_key(primary_key,last_tid,tid,s_w_id[last_tid],s_i_id[last_tid]);
		s_i_id[tid] = s_i_id[last_tid];
		s_w_id[tid] = s_w_id[last_tid];
		s_quantity[tid] = s_quantity[last_tid];
		s_dist_01[tid] = s_dist_01[last_tid];
		s_dist_02[tid] = s_dist_02[last_tid];
		s_dist_03[tid] = s_dist_03[last_tid];
		s_dist_04[tid] = s_dist_04[last_tid];
		s_dist_05[tid] = s_dist_05[last_tid];
		s_dist_06[tid] = s_dist_06[last_tid];
		s_dist_07[tid] = s_dist_07[last_tid];
		s_dist_08[tid] = s_dist_08[last_tid];
		s_dist_09[tid] = s_dist_09[last_tid];
		s_dist_10[tid] = s_dist_10[last_tid];
		s_ytd[tid] = s_ytd[last_tid];
		s_order_cnt[tid] = s_order_cnt[last_tid];
		s_remote_cnt[tid] = s_remote_cnt[last_tid];
		s_data[tid] = s_data[last_tid];
	}
	s_i_id.pop_back();
	s_w_id.pop_back();
	s_quantity.pop_back();
	s_dist_01.pop_back();
	s_dist_02.pop_back();
	s_dist_03.pop_back();
	s_dist_04.pop_back();
	s_dist_05.pop_back();
	s_dist_06.pop_back();
	s_dist_07.pop_back();
	s_dist_08.pop_back();
	s_dist_09.pop_back();
	s_dist_10.pop_back();
	s_ytd.pop_back();
	s_order_cnt.pop_back();
	s_remote_cnt.pop_back();
	s_data.pop_back();
}

void Table_stock::read_from_file(ifstream& in)
{
	assert(in.is_open());
	string buf_field; buf_field.reserve(500);
	Integer in_s_i_id;
	Integer in_s_w_id;
	Numeric<4,0> in_s_quantity;
	Char<24> in_s_dist_01;
	Char<24> in_s_dist_02;
	Char<24> in_s_dist_03;
	Char<24> in_s_dist_04;
	Char<24> in_s_dist_05;
	Char<24> in_s_dist_06;
	Char<24> in_s_dist_07;
	Char<24> in_s_dist_08;
	Char<24> in_s_dist_09;
	Char<24> in_s_dist_10;
	Numeric<8,0> in_s_ytd;
	Numeric<4,0> in_s_order_cnt;
	Numeric<4,0> in_s_remote_cnt;
	Varchar<50> in_s_data;
	char first_symbol = 0;
	while (!in.eof()) {
		first_symbol = in.get();
		if (first_symbol == ROW_DLM || first_symbol == EOF) continue;
		in.unget();
		assert(!in.fail());

		getline(in, buf_field, FLD_DLM); in_s_i_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_w_id = Integer::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_quantity = Numeric<4,0>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_dist_01 = Char<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_dist_02 = Char<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_dist_03 = Char<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_dist_04 = Char<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_dist_05 = Char<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_dist_06 = Char<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_dist_07 = Char<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_dist_08 = Char<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_dist_09 = Char<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_dist_10 = Char<24>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_ytd = Numeric<8,0>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_order_cnt = Numeric<4,0>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, FLD_DLM); in_s_remote_cnt = Numeric<4,0>::castString(buf_field.data(), buf_field.length());
		getline(in, buf_field, ROW_DLM); in_s_data = Varchar<50>::castString(buf_field.data(), buf_field.length());

		insert(in_s_i_id,in_s_w_id,in_s_quantity,in_s_dist_01,in_s_dist_02,in_s_dist_03,in_s_dist_04,in_s_dist_05,in_s_dist_06,in_s_dist_07,in_s_dist_08,in_s_dist_09,in_s_dist_10,in_s_ytd,in_s_order_cnt,in_s_remote_cnt,in_s_data);
	}
}



