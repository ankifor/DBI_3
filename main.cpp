#include <atomic>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>


#include <iostream>
#include <fstream>
#include <chrono>
#include "Types.hpp"
#include "schema_1.hpp"

using namespace std;

Table_warehouse  warehouse;
Table_district   district;
Table_customer   customer;
Table_history    history;
Table_neworder   neworder;
Table_order      order;
Table_orderline  orderline;
Table_item       item;
Table_stock      stock;

extern void oltp(Timestamp now);
extern void deliveryRandom(Timestamp now);
extern void newOrderRandom(Timestamp now);
extern Numeric<12,2> join_query();


atomic<bool> childRunning;
atomic<pid_t> pid;
atomic<bool> calculationFinished;
chrono::duration<int64_t, std::micro> elapsed_otlp;
chrono::duration<int64_t, std::micro> elapsed_fork;
chrono::duration<int64_t, std::micro>* elapsed_query;
size_t num_query;


static void child_main() {
	Numeric<12,2> sum = 0;
	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	try {
		sum = join_query();
	} catch (const exception& e) {
		sum = 0;
		cout << e.what() << endl;
	}
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
	*elapsed_query += chrono::duration_cast<chrono::microseconds>(end - start);
	cout << sum << ";ms:" << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
	exit(0);
}

static void SIGCHLD_handler(int /*sig*/) {
	int status;
	pid_t childPid = wait(&status);
	// now the child with process id childPid is dead
	childRunning=false;
	if (calculationFinished) return;
	++num_query;
	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	pid = fork();
	if (pid) {
		chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
		elapsed_fork += chrono::duration_cast<chrono::microseconds>(end - start);
		return;
	} else {
		child_main();
		exit(0);//actually is never reached
	}
}



static void read_data(const string& path)
{
	ifstream in;

	in.open(path + "tpcc_warehouse.tbl");
	warehouse.read_from_file(in);
	in.close();

	in.open(path + "tpcc_district.tbl");
	district.read_from_file(in);
	in.close();

	in.open(path + "tpcc_customer.tbl");
	customer.read_from_file(in);
	in.close();

	in.open(path + "tpcc_history.tbl");
	history.read_from_file(in);
	in.close();

	in.open(path + "tpcc_order.tbl");
	order.read_from_file(in);
	in.close();

	in.open(path + "tpcc_neworder.tbl");
	neworder.read_from_file(in);
	in.close();

	in.open(path + "tpcc_orderline.tbl");
	orderline.read_from_file(in);
	in.close();

	in.open(path + "tpcc_item.tbl");
	item.read_from_file(in);
	in.close();

	in.open(path + "tpcc_stock.tbl");
	stock.read_from_file(in);
	in.close();
}

static int test(int argc, char **argv) {
	ifstream in;
	const string path_in = argv[1];
	in.open(path_in + "tpcc_order.tbl");
	order.read_from_file(in);
	in.close();	
	
	assert(order.size() < 10);
	
	for (auto& it : order.primary_key) cout << it.first << ":" << it.second << endl;
	for (auto& it : order.order_wdc)   cout << it.first << ":" << it.second << endl;
	cout << endl;
	
	order.remove(4);
	
	for (auto& it : order.primary_key) cout << it.first << ":" << it.second << endl;
	for (auto& it : order.order_wdc)   cout << it.first << ":" << it.second << endl;
	cout << endl;

	order.insert(5,1,4,1335,Timestamp(39),1,6,1);

	for (auto& it : order.primary_key) cout << it.first << ":" << it.second << endl;
	for (auto& it : order.order_wdc)   cout << it.first << ":" << it.second << endl;
	cout << endl;

	return 0;
}


int main0(int argc, char **argv)
{
	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point end;
	
	if (argc < 2) {
		cerr << "Wrong call, please, use:" << endl
			 << argv[0] << " <path_to_tables>" << endl;
		return 1;
	}
	string path_in = argv[1]; path_in += "/";

	read_data(path_in);

	// init globals
	elapsed_otlp = chrono::duration<int64_t, std::micro>::zero();
	elapsed_fork = chrono::duration<int64_t, std::micro>::zero();
	childRunning = false;
	calculationFinished = false;
	num_query = 0;
	
	
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;
	sa.sa_handler=SIGCHLD_handler;
	sigaction(SIGCHLD,&sa,NULL);
	
	elapsed_query = static_cast<decltype(elapsed_query)>(mmap(NULL, sizeof *elapsed_query, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
	*elapsed_query = chrono::duration<int64_t, std::micro>::zero();

	size_t n = 1000000;

	
	//{int i; cout << "..."; cin >> i; cout << endl;}
	
	// create a child; all another children are created from SIGCHLD_handler
	childRunning = true;
	++num_query;
	start = chrono::high_resolution_clock::now();
	pid = fork();
	if (pid) {
		end = chrono::high_resolution_clock::now();
		elapsed_fork += chrono::duration_cast<chrono::microseconds>(end - start);
	} else {
		child_main();
		exit(0);//actually is never reached
	}
	
	// main oltp-loop
	for (size_t i = 0; i < n; ++i) {
		start = chrono::high_resolution_clock::now();
		try {
			oltp(0);
		} catch (const exception& e) {
			cout << e.what() << endl;
		}
		end = chrono::high_resolution_clock::now();
		elapsed_otlp += chrono::duration_cast<chrono::microseconds>(end - start);
	}
	//finished: wait for child
	calculationFinished = true;
	while (childRunning) {
		usleep(100);//wait 100 ms
	}
	
	
	//{int i; cout << "..."; cin >> i; cout << endl;}
	// print statistics
	auto s_oltp   = (chrono::duration_cast<chrono::seconds>(elapsed_otlp))      .count();
	auto ms_fork  = (chrono::duration_cast<chrono::milliseconds>(elapsed_fork)) .count();
	auto ms_query = (chrono::duration_cast<chrono::milliseconds>(*elapsed_query)).count();
	cout << double(n)        / double(s_oltp)    << " oltp per sec" << endl //transactions per second
	     << double(ms_fork)  / double(num_query) << " ms per fork"  << endl //ms per fork
	     << double(ms_query) / double(num_query) << " ms per query" << endl;//ms per query 
	
	munmap(elapsed_query, sizeof *elapsed_query);
	return 0;
}

int main1(int argc, char **argv)
{
	if (argc < 2) {
		cerr << "Wrong call, please, use:" << endl
			 << argv[0] << " <path_to_tables>" << endl;
		return 1;
	}
	string path_in = argv[1]; path_in += "/";

	read_data(path_in);

	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point end;
	chrono::duration<int64_t, std::micro> elapsed = chrono::duration<int64_t, std::micro>::zero();


	size_t n = 10;
	Numeric<12,2> sum;
	{int i; cout << "..."; cin >> i; cout << endl;}
	for (size_t i = 0; i < n; ++i) {
		start = chrono::high_resolution_clock::now();
		try {
			sum = join_query();
		} catch (const exception& e) {
			cout << e.what() << endl;
		}
		end = chrono::high_resolution_clock::now();
		elapsed += chrono::duration_cast<chrono::microseconds>(end - start);
		cout << sum << ";ms:"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
	}
	
	{int i; cout << "..."; cin >> i; cout << endl;}
	auto m = (chrono::duration_cast<chrono::milliseconds>(elapsed)).count();
	cout << m << ' '                         //ms total
	    << double(m) / double(n)  << endl;   //ms per transaction

	return 0;
}

bool pred(const Varchar<16>& s) { return s.len > 0 && s.value[0] == 'B'; }

void run_query() {
  using type_tuple = tuple<Integer, Integer, Integer>;
  using type_tids = tuple<Tid, Tid>;
  unordered_multimap<type_tuple, type_tids, hash_types::hash<type_tuple>> hash;
  using type_tuple1 = tuple<Integer, Integer, Integer>;
  using type_tids1 = tuple<Tid>;
  unordered_multimap<type_tuple1, type_tids1, hash_types::hash<type_tuple1>>
      hash1;
  for (Tid tid = 0; tid < customer.size(); ++tid) {
    if (pred(customer.c_last[tid])) {
      auto t = make_tuple(customer.c_w_id[tid], customer.c_d_id[tid],
                          customer.c_id[tid]);
      auto t_tids = make_tuple(tid);
      hash1.insert(make_pair(t, t_tids));
    }
  }
  for (Tid tid1 = 0; tid1 < order.size(); ++tid1) {
    auto t =
        make_tuple(order.o_w_id[tid1], order.o_d_id[tid1], order.o_c_id[tid1]);
    for (auto it = hash1.equal_range(t); it.first != it.second; ++it.first) {
      auto tid = get<0>(it.first->second);
      auto t =
          make_tuple(order.o_w_id[tid1], order.o_d_id[tid1], order.o_id[tid1]);
      auto t_tids = make_tuple(tid, tid1);
      hash.insert(make_pair(t, t_tids));
    }
  }
  int count = 0;
  for (Tid tid2 = 0; tid2 < orderline.size() && count < 10; ++tid2) {
    auto t = make_tuple(orderline.ol_w_id[tid2], orderline.ol_d_id[tid2],
                        orderline.ol_o_id[tid2]);
    for (auto it = hash.equal_range(t); it.first != it.second; ++it.first) {
      auto tid = get<0>(it.first->second);
      auto tid1 = get<1>(it.first->second);
      cout << customer.c_id[tid] << "," << order.o_id[tid1] << ","
           << orderline.ol_o_id[tid2] << "," << orderline.ol_d_id[tid2] << ","
           << orderline.ol_w_id[tid2] << "," << orderline.ol_number[tid2]
<< endl;
		++count;
    }
  }
}


int main(int argc, char **argv)
{
	if (argc < 2) {
		cerr << "Wrong call, please, use:" << endl
			 << argv[0] << " <path_to_tables>" << endl;
		return 1;
	}
	string path_in = argv[1]; path_in += "/";

	read_data(path_in);

	run_query();

	return 0;
}