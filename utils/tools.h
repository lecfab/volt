#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <fstream>      // ofstream: write, ifstream: read, fstream: read and write from/to files
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <climits>      // UINT_MAX, ULONG_MAX... https://cplusplus.com/reference/climits/
#include <string>
#include <chrono>
#include <cmath>
#include <vector>
#include <utility>
#include <list>

#ifndef NLINKS
  #define NNODES 10000000   // estimated number of nodes (10^7)
  #define NLINKS 100000000  // estimated number of edges (10^8)
  #define MAXNODES 4294967290 // max possible number of edges (2^32-6)
  #define MAXLINKS 9223300000000000000 // max possible number of edges (~2^63)
#endif

// todo: rename type ul into intNode
#if MAXNODES < UINT_MAX
  #define ULPRINTF "u"
  typedef unsigned int ul; // up to 2^32 nodes
#else
  #define ULPRINTF "lu"
  typedef unsigned long ul; // over 4 billion nodes
#endif

#if MAXLINKS < UINT_MAX
  typedef unsigned int intEdge;
#else
  typedef unsigned long intEdge;
#endif

typedef unsigned long long ull;

#define MSGOUT std::cerr
#define MSGflush stderr

#ifndef DEBUG
  #define Debug(msg) {}
#else
  #define Debug(msg) { MSGOUT << "\tDebug ("<<__FILE__<<":"<<__LINE__<<") " << msg << std::endl; fflush(MSGflush); }
#endif
#define Alert(msg) { MSGOUT << "\tAlert ("<<__FILE__<<":"<<__LINE__<<") "<< msg << std::endl; fflush(MSGflush); }
#define Info(msg) { MSGOUT << "\tInfo: " << msg << std::endl; fflush(MSGflush); }


#define TimeBegin() auto time_begin = std::chrono::steady_clock::now(); \
                    auto time_previous = time_begin, timeRec_previous = time_begin, time_now = time_begin; \
                    std::time_t t0, t1, t2; t1 = std::time(nullptr); t0 = t1;
                    // auto clock_begin = std::clock();
                    // auto clock_previous = clock_begin, clockRec_previous = clock_begin, clock_now = clock_begin;
                    // auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_previous).count();
                    // auto clock_diff = clock_now - clock_begin;
#define TimeStep(m) MSGOUT << m; \
                    time_now = std::chrono::steady_clock::now(); \
                    t2 = time(NULL); \
                    print_chrono_ctime(time_previous, time_now, t1, t2); \
                    time_previous = time_now; \
                    t1 = t2;
                    // clock_now = std::clock();
                    // clock_previous = clock_now;
                    // print_chrono_clock(time_previous, time_now, clock_previous, clock_now);
#define TimeTotal()	MSGOUT << "Total"; \
                    time_now = std::chrono::steady_clock::now(); \
                    t2 = time(NULL); \
                    print_chrono_ctime(time_begin, time_now, t0, t2);
                    // clock_now = std::clock();
                    // print_chrono_clock(time_begin, time_now, clock_begin, clock_now);

#define TimeRecStep(m, out) time_now = std::chrono::steady_clock::now(); \
                            t2 = time(NULL); \
                            out << m << "\t"; \
                            out << std::chrono::duration_cast<std::chrono::milliseconds>(time_now - timeRec_previous).count() << "\t"; \
                            out << t2 - t1 << endl; \
                            timeRec_previous = time_now; \
                            t1 = t2;
                            // clock_now = std::clock();
                            // clockRec_previous = clock_now;
                            // out << clock_now - clockRec_previous << endl;
#define TimeRecTotal(m, out)  time_now = std::chrono::steady_clock::now(); \
                              t2 = time(NULL); \
                              out << m << "\t"; \
                              out << std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_begin).count() << "\t"; \
                              out << t2 - t0 << endl;
                              // clock_now = std::clock();
                              // out << clock_now - clock_begin << endl;

// #define TimeRecStep(m) time_now = chrono::steady_clock::now(); MSGOUT << chrono::duration_cast<chrono::milliseconds>(time_now - time_previous).count() << "\t"; time_previous = time_now; Debug(m)
// #define TimeRecTotal() time_now = chrono::steady_clock::now(); MSGOUT << chrono::duration_cast<chrono::milliseconds>(time_now - time_begin).count() << endl;

// #define TimeRecBegin() vector<chrono::steady_clock::time_point> time_rec; time_rec.push_back(chrono::steady_clock::now());
// #define TimeRecStep(m) time_rec.push_back(chrono::steady_clock::now());
// #define TimeRecTotal() { auto t0 = time_rec[0], t1=t0; for(auto &t : time_rec) { MSGOUT << chrono::duration_cast<chrono::milliseconds>(t - t1).count() << ","; t1=t; } MSGOUT << chrono::duration_cast<chrono::milliseconds>(t1 - t0).count() << endl; }

void print_chrono_ctime(
  std::chrono::steady_clock::time_point begin,
  std::chrono::steady_clock::time_point end,
  std::time_t t_begin,
  std::time_t t_end);

// https://www.cprogramming.com/c++11/c++11-auto-decltype-return-value-after-function.html
// template <typename Builder>
// auto makeAndProcessObject (const Builder& builder) -> decltype( builder.makeObject() );
// template <typename T>
// auto Timer (T &function, int argc, char** argv) -> decltype(function(argc, argv));


//compute the maximum of three unsigned long
inline ul max3(ul a, ul b, ul c) { return (a >= b and a >= c) ? a : ((b >= c) ? b : c); }

struct Keyvalue {
	ul key;
	ul val;
	Keyvalue(ul key, ul val) : key(key), val(val) {}
};

std::vector<ul> order_identity(const ul &n);
std::vector<ul> rank_from_order(const std::vector<ul> &order, const ul &n);
std::vector<ul> rank_from_order(const std::vector<ul> &order);
std::pair<ul,ul> random_pair(const ul &n);

#endif
