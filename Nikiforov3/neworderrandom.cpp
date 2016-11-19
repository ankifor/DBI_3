#include <cstdint>
#include <cstdlib>
#include <unordered_map>
#include "Types.hpp"
#include "schema_1.hpp"
#include "Help.hpp"

#include <iostream>

static const int32_t warehouses=5;

static int32_t urand(int32_t min,int32_t max) {
   return (rand()%(max-min+1))+min;
}

static int32_t urandexcept(int32_t min,int32_t max,int32_t v) {
	if (max<=min)
		return min;
	int32_t r=(rand()%(max-min))+min;
	if (r>=v)
		return r+1; 
	else
		return r;
}

static int32_t nurand(int32_t A,int32_t x,int32_t y) {
   return ((((rand()%A)|(rand()%(y-x+1)+x))+42)%(y-x+1))+x;
}

//create transaction newOrder (
//  integer w_id
//, integer d_id
//, integer c_id
//, integer items
//, array(15) integer supware
//, array(15) integer itemid
//, array(15) integer qty
//, timestamp datetime
//)
static void newOrder(
	  int32_t   in_w_id
	, int32_t   in_d_id
	, int32_t   in_c_id
	, int32_t   in_items
	, int32_t*  in_supware
	, int32_t*  in_itemid
	, int32_t*  in_qty
	, Timestamp in_datetime
)
{	
	Tid tid;
	//select w_tax from warehouse w where w.w_id=w_id;	
	Numeric<4,4> w_tax;
	{
		auto it = warehouse.primary_key.find(make_tuple(in_w_id));
		if (it != warehouse.primary_key.end()) {
			w_tax = warehouse.w_tax[it->second];
		}
	}
	//select c_discount from customer c where c_w_id=w_id and c_d_id=d_id and c.c_id=c_id;
	Numeric<4,4> c_discount;
	{
		auto it = customer.primary_key.find(make_tuple(in_w_id,in_d_id,in_c_id));
		if (it != customer.primary_key.end()) {
			c_discount = customer.c_discount[it->second];
		}
	}
	//select d_next_o_id as o_id,d_tax from district d where d_w_id=w_id and d.d_id=d_id;
	Integer o_id;
	Numeric<4,4> d_tax;
	{
		auto it = district.primary_key.find(make_tuple(in_w_id,in_d_id));
		if (it != district.primary_key.end()) {
			o_id = district.d_next_o_id[it->second];
			d_tax = district.d_tax[it->second];
		}
	}

	//update district set d_next_o_id=o_id+1 where d_w_id=w_id and district.d_id=d_id;
	{
		auto it = district.primary_key.find(make_tuple(in_w_id,in_d_id));
		if (it != district.primary_key.end()) {
			district.d_next_o_id[it->second] = o_id + 1;
		}
	}

	//var integer all_local = 1;
	//forsequence (index between 0 and items-1) {
	//   if (w_id<>supware[index])
	//      all_local=0;
	//}
	Numeric<1,0> all_local(1);
	for (int32_t index = 0; index < in_items; ++index) {
		if (in_w_id != in_supware[index]) {
			all_local = 0;
		}
	}
	//insert into "order" values (o_id,d_id,w_id,c_id,datetime,0,items,all_local);	
	order.insert(o_id,in_d_id,in_w_id,in_c_id,in_datetime,0,in_items,all_local);
	//insert into neworder values (o_id,d_id,w_id);
	neworder.insert(o_id,in_d_id,in_w_id);

	vector<Char<24> >* stock_s_dist = nullptr;
	switch (in_d_id) {
		case  1: stock_s_dist = &stock.s_dist_01; break;
		case  2: stock_s_dist = &stock.s_dist_02; break;
		case  3: stock_s_dist = &stock.s_dist_03; break;
		case  4: stock_s_dist = &stock.s_dist_04; break;
		case  5: stock_s_dist = &stock.s_dist_05; break;
		case  6: stock_s_dist = &stock.s_dist_06; break;
		case  7: stock_s_dist = &stock.s_dist_07; break;
		case  8: stock_s_dist = &stock.s_dist_08; break;
		case  9: stock_s_dist = &stock.s_dist_09; break;
		case 10: stock_s_dist = &stock.s_dist_10; break;
		default: stock_s_dist = nullptr         ; break;
	}
	
	//forsequence (index between 0 and items-1) {
	for (int32_t index = 0; index < in_items; ++index) {
        tid = -1;
		//select i_price from item where i_id=itemid[index];
		Numeric<5,2> i_price;
		{
			auto it = item.primary_key.find(make_tuple(in_itemid[index]));
			if (it != item.primary_key.end()) {
				i_price = item.i_price[it->second];
			}
		}		
		//select s_quantity,s_remote_cnt,s_order_cnt,case d_id when 1 then s_dist_01 when 2 then s_dist_02 when 3 then s_dist_03 when 4 then s_dist_04 when 5 then s_dist_05 when 6 then s_dist_06 when 7 then s_dist_07 when 8 then s_dist_08 when 9 then s_dist_09 when 10 then s_dist_10 end as s_dist 
		//from stock where s_w_id=supware[index] and s_i_id=itemid[index];
		Numeric<4,0> s_quantity;
		Numeric<4,0> s_remote_cnt;
		Numeric<4,0> s_order_cnt;
		Char<24> s_dist;
		{
			auto it = stock.primary_key.find(make_tuple(in_supware[index], in_itemid[index]));
			if (it != stock.primary_key.end()) {
				Tid id = it->second;
				s_quantity = stock.s_quantity[id];
				s_remote_cnt = stock.s_remote_cnt[id];
				s_order_cnt = stock.s_order_cnt[id];
				if (stock_s_dist != nullptr) s_dist = (*stock_s_dist)[id];
				tid = id;
			}
		}		
		//if (s_quantity>qty[index]) {
		//   update stock set s_quantity=s_quantity-qty[index] where s_w_id=supware[index] and s_i_id=itemid[index];
		//} else {
		//   update stock set s_quantity=s_quantity+91-qty[index] where s_w_id=supware[index] and s_i_id=itemid[index];
		//}
		if (s_quantity > in_qty[index]) {			
			auto it = stock.primary_key.find(make_tuple(in_supware[index], in_itemid[index]));
			if (it != stock.primary_key.end()) {
				stock.s_quantity[it->second] += -in_qty[index];
			}	
		} else {
			auto it = stock.primary_key.find(make_tuple(in_supware[index], in_itemid[index]));
			if (it != stock.primary_key.end()) {
				stock.s_quantity[it->second] += 91-in_qty[index];
			}			
		}
		//if (supware[index]<>w_id) {
		//   update stock set s_remote_cnt=s_remote_cnt+1 where s_w_id=w_id and s_i_id=itemid[index];
		//} else {
		//   update stock set s_order_cnt=s_order_cnt+1 where s_w_id=w_id and s_i_id=itemid[index];
		//}
		if (in_supware[index] != in_w_id) {
			auto it = stock.primary_key.find(make_tuple(in_supware[index], in_itemid[index]));
			if (it != stock.primary_key.end()) {
				stock.s_remote_cnt[it->second] += 1;
			}
		} else {
			auto it = stock.primary_key.find(make_tuple(in_supware[index], in_itemid[index]));
			if (it != stock.primary_key.end()) {
				stock.s_order_cnt[it->second] += 1;
			}
		}
		//var numeric(6,2) ol_amount=qty[index]*i_price*(1.0+w_tax+d_tax)*(1.0-c_discount);
		Numeric<6,2> ol_amount = 0;
		//	(
		//	(Numeric<5,0>(in_qty[index]).cast<5,2>()*i_price).cast<6,8>() //
		//	* ((Numeric<4,0>(1).cast<4,4>() + w_tax + d_tax)
		//	* (Numeric<4,0>(1).cast<4,4>() - c_discount)).cast<6,8>()
		//	).cast<6,2>()
		//	;
		//insert into orderline values (o_id,d_id,w_id,index+1,itemid[index],supware[index],0,qty[index],ol_amount,s_dist);
		orderline.insert(o_id,in_d_id,in_w_id,index+1,in_itemid[index],in_supware[index],0,in_qty[index],ol_amount,s_dist);
	//}
	}
	//commit;
	return;
}

//create transaction delivery(integer w_id, integer o_carrier_id, timestamp datetime)
//{
static void delivery(Integer w_id, Integer o_carrier_id, Timestamp datetime) 
{
//   forsequence (d_id between 1 and 10) {
	for (Integer d_id = 1; d_id <= 10; d_id += 1) {
//      select min(no_o_id) as o_id from neworder where no_w_id=w_id and no_d_id=d_id order by no_o_id else { continue; } -- ignore this district if no row found
		Integer o_id;
		{
			//todo primary_key: no_w_id, no_d_id, no_o_id
			auto it = neworder.primary_key.lower_bound(make_tuple(w_id,d_id,Integer::Min()));
			if (it != neworder.primary_key.end() && get<0>(it->first) == w_id && get<1>(it->first) == d_id ) {
				o_id = neworder.no_o_id[it->second];
			}  else {
				continue;
			}
		}
//      delete from neworder where no_w_id=w_id and no_d_id=d_id and no_o_id=o_id;
		{
			auto it = neworder.primary_key.find(make_tuple(w_id,d_id,o_id));
			if (it != neworder.primary_key.end()) {
				neworder.remove(it->second);
			}
		}
//      select o_ol_cnt,o_c_id from "order" o where o_w_id=w_id and o_d_id=d_id and o.o_id=o_id;
		Numeric<2,0> o_ol_cnt;
		Integer o_c_id;
		{
			auto it = order.primary_key.find(make_tuple(w_id,d_id,o_id));
			if (it != order.primary_key.end()) {
				o_ol_cnt = order.o_ol_cnt[it->second];
				o_c_id   = order.o_c_id  [it->second];
			}
		}
//      update "order" set o_carrier_id=o_carrier_id where o_w_id=w_id and o_d_id=d_id and "order".o_id=o_id;
		{
			auto it = order.primary_key.find(make_tuple(w_id,d_id,o_id));
			if (it != order.primary_key.end()) {
				 order.o_carrier_id[it->second] = o_carrier_id;
			}
		}
//      var numeric(6,2) ol_total=0;
		Numeric<6,2> ol_total = 0;
//      forsequence (ol_number between 1 and o_ol_cnt) {
		for (uint32_t ol_number = 1; o_ol_cnt >= ol_number; ol_number += 1) {
			Numeric<6,2> ol_amount = 0;
//         select ol_amount from orderline where ol_w_id=w_id and ol_d_id=d_id and ol_o_id=o_id and orderline.ol_number=ol_number;
			{
				auto it = orderline.primary_key.find(make_tuple(w_id,d_id,o_id,ol_number));
				if (it != orderline.primary_key.end()) {
					 ol_amount = orderline.ol_amount[it->second];
				}
			}
//         ol_total=ol_total+ol_amount;
			ol_total += ol_amount;
//         update orderline set ol_delivery_d=datetime where ol_w_id=w_id and ol_d_id=d_id and ol_o_id=o_id and orderline.ol_number=ol_number;
			{
				auto it = orderline.primary_key.find(make_tuple(w_id,d_id,o_id,ol_number));
				if (it != orderline.primary_key.end()) {
					 orderline.ol_delivery_d[it->second] = datetime;
				}
			}
//      }
		}
//      update customer set c_balance=c_balance+ol_total where c_w_id=w_id and c_d_id=d_id and c_id=o_c_id;
		{
			auto it = customer.primary_key.find(make_tuple(w_id,d_id,o_c_id));
			if (it != customer.primary_key.end()) {
				customer.c_balance[it->second] = customer.c_balance[it->second] + ol_total.cast<12,2>();
			}
		}
//   }
	}
	//cout << count << endl;
//   commit;
//};
}


void newOrderRandom(Timestamp now) {
	int32_t w_id=urand(1,warehouses);
	int32_t d_id=urand(1,1);
	int32_t c_id=nurand(1023,1,3000);
	int32_t ol_cnt=urand(5,15);

	int32_t supware[15];
	int32_t itemid[15];
	int32_t qty[15];
	for (int32_t i=0; i<ol_cnt; i++) {
		if (urand(1,100)>1)
			supware[i]=w_id; 
		else
			supware[i]=urandexcept(1,warehouses,w_id);
		itemid[i]=nurand(8191,1,100000);
		qty[i]=urand(1,10);
	}

	newOrder(w_id,d_id,c_id,ol_cnt,supware,itemid,qty,now);
}

void deliveryRandom(Timestamp now) {
	int32_t w_id=urand(1,warehouses);
	int32_t o_carrier_id = urand(1,10);
	delivery(w_id,o_carrier_id,now);
}

void oltp(Timestamp now) {
	int rnd=urand(1,100);
	if (rnd<=10) {
		deliveryRandom(now);
	} else {
		newOrderRandom(now);
	}
}
