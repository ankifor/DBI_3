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
extern Numeric<12,4> join_query();
extern Numeric<12,4> join_query1();
extern Numeric<12,4> join_query2();


atomic<bool> childRunning;
atomic<pid_t> pid;
atomic<bool> calculationFinished;
chrono::duration<int64_t, std::micro> elapsed_otlp;
chrono::duration<int64_t, std::micro> elapsed_fork;
chrono::duration<int64_t, std::micro>* elapsed_query;
size_t num_query;


static void child_main() {
	Numeric<12,4> sum = 0;
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

//	in.open(path + "tpcc_warehouse.tbl");
//	warehouse.read_from_file(in);
//	in.close();
//
//	in.open(path + "tpcc_district.tbl");
//	district.read_from_file(in);
//	in.close();

	in.open(path + "tpcc_customer.tbl");
	customer.read_from_file(in);
	in.close();

//	in.open(path + "tpcc_history.tbl");
//	history.read_from_file(in);
//	in.close();

	in.open(path + "tpcc_order.tbl");
	order.read_from_file(in);
	in.close();

//	in.open(path + "tpcc_neworder.tbl");
//	neworder.read_from_file(in);
//	in.close();

	in.open(path + "tpcc_orderline.tbl");
	orderline.read_from_file(in);
	in.close();

//	in.open(path + "tpcc_item.tbl");
//	item.read_from_file(in);
//	in.close();
//
//	in.open(path + "tpcc_stock.tbl");
//	stock.read_from_file(in);
//	in.close();
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

int main(int argc, char **argv)
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
	chrono::duration<int64_t, std::micro> elapsed0 = chrono::duration<int64_t, std::micro>::zero();


	size_t n = 10;
	Numeric<12,4> sum;
	{int i; cout << "..."; cin >> i; cout << endl;}
	for (size_t i = 0; i < n; ++i) {
		start = chrono::high_resolution_clock::now();
		try {
			sum = join_query1();
		} catch (const exception& e) {
			cout << e.what() << endl;
		}
		end = chrono::high_resolution_clock::now();
		elapsed0 += chrono::duration_cast<chrono::microseconds>(end - start);
		cout << "1:" << sum << ";ms:"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;
			
			
		start = chrono::high_resolution_clock::now();
		try {
			sum = join_query2();
		} catch (const exception& e) {
			cout << e.what() << endl;
		}
		end = chrono::high_resolution_clock::now();
		elapsed += chrono::duration_cast<chrono::microseconds>(end - start);
		cout << "2:" << sum << ";ms:"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl;			
	}
	
	{int i; cout << "..."; cin >> i; cout << endl;}
	auto m = (chrono::duration_cast<chrono::milliseconds>(elapsed)).count();
	cout << "avg 2:" << double(m) / double(n)  << endl;   //ms per transaction
	m = (chrono::duration_cast<chrono::milliseconds>(elapsed0)).count();
	cout << "avg 1:" << double(m) / double(n)  << endl;   //ms per transaction

	return 0;
}
