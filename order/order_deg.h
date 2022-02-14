#ifndef ORDER_DEG_H
#define ORDER_DEG_H

#include "../utils/tools.h"

struct Keyvalue;
struct Edgelist;
struct Adjlist;

inline bool compare_nodedeg_desc (Keyvalue &a, Keyvalue &b);
inline bool compare_nodedeg_asc (Keyvalue &a, Keyvalue &b);

// ----------------- From edgelist ------------------
std::vector<ul> rank_from_deg(const std::vector<ul> &deg, const ul &n, bool desc=true, bool order=false);
std::vector<ul> order_from_deg(const std::vector<ul> &deg, const ul &n, bool desc=true);
std::vector<ul> order_deg(Edgelist &g, bool desc=true);
std::vector<ul> order_degOut(Edgelist &g, bool desc=true);
std::vector<ul> order_degIn(Edgelist &g, bool desc=true);
std::vector<ul> order_deg_split(Edgelist &g, bool desc=true);

// ----------------- From adjlist -------------------
std::vector<ul> order_degOut(Adjlist &g);
std::vector<ul> order_degIn(Adjlist &g);

#endif

// g++ -std=c++14  benchmark.o utils/tools.o utils/inout.o utils/edgelist.o utils/adjlist.o utils/heap.o utils/continuousrank.o utils/unitheap.o algo/algo_nq.o algo/algo_bfs.o algo/algo_dfs.o algo/algo_bellman.o algo/algo_diameter.o algo/algo_kcore.o algo/algo_tarjan.o algo/algo_pagerank.o algo/algo_dominatingset.o algo/algo_minuncut.o algo/algo_triangles.o order/order_deg.o order/order_rand.o order/order_rcm.o order/order_gorder.o order/order_ldg.o order/order_minla.o order/order_triangles.o order/order_slashburn.o -Ofast -g -fopenmp  -o benchmark
// /usr/bin/ld : algo/algo_minuncut.o : dans la fonction « order_from_deg(std::vector<unsigned long, std::allocator<unsigned long> > const&, unsigned long const&, bool) » :
// /home/fab/Documents/these/work/src/algo/../order/order_deg.h:15 : définitions multiples de « order_from_deg(std::vector<unsigned long, std::allocator<unsigned long> > const&, unsigned long const&, bool) »; benchmark.o:/home/fab/Documents/these/work/src/order/order_deg.h:15 : défini pour la première fois ici
// /usr/bin/ld : order/order_deg.o : dans la fonction « order_from_deg(std::vector<unsigned long, std::allocator<unsigned long> > const&, unsigned long const&, bool) » :
// /home/fab/Documents/these/work/src/order/order_deg.h:15 : définitions multiples de « order_from_deg(std::vector<unsigned long, std::allocator<unsigned long> > const&, unsigned long const&, bool) »; benchmark.o:/home/fab/Documents/these/work/src/order/order_deg.h:15 : défini pour la première fois ici
// /usr/bin/ld : order/order_rcm.o : dans la fonction « __gnu_cxx::new_allocator<unsigned long>::deallocate(unsigned long*, unsigned long) » :
// /home/fab/Documents/these/work/src/order/order_deg.h:15 : définitions multiples de « order_from_deg(std::vector<unsigned long, std::allocator<unsigned long> > const&, unsigned long const&, bool) »; benchmark.o:/home/fab/Documents/these/work/src/order/order_deg.h:15 : défini pour la première fois ici
// collect2: error: ld returned 1 exit status
// make: *** [makefile:52 : benchmark] Erreur 1
