#ifndef ADJLIST_H
#define ADJLIST_H

/* Structures for compact adjacency lists, built from a list of edges */

#include "tools.h"
#include <functional>
struct Edgelist;


struct NeighIter {
  const ul *begin_value;
  const ul *end_value;
  NeighIter(const ul *begin_value, const ul *end_value) : begin_value(begin_value), end_value(end_value) {}
  const ul* begin() const { return begin_value; }
  const ul* end() const { return end_value; }
};

struct Adjlist {
public:
  bool directed;
	int edge_factor;
  int node_factor;
	ul n;                //number of nodes
	intEdge e;           //number of edges
	std::vector<intEdge> cd;  //cumulative degree cd[0]=0, length=n+1
	std::vector<ul> adj; //concatenated lists of neighbours of all nodes, length e
  std::function<ul(ul)> ranker;
  void ranker_reset();

  Adjlist(Edgelist &g, int edge_factor=1, int node_factor=1);

	// iterate over neighbours: for (auto v=neigh_beg(u); v<neigh_end(u); ++v)  cout << *v << endl;
  // or with iterator: for (auto &v : g.neigh_iter(u)) cout << v << endl;
  inline ul* neigh_beg(const ul &u) { return &adj[cd[u]]; }
  inline const ul* neigh_beg(const ul &u) const { return &adj[cd[u]]; }
  inline ul* neigh_end(const ul &u) { return &adj[cd[u+1]]; }
  inline const ul* neigh_end(const ul &u) const { return &adj[cd[u+1]]; }
  inline NeighIter neigh_iter(const ul &u) { return NeighIter(neigh_beg(u), neigh_end(u)); }
  inline const NeighIter neigh_iter(const ul &u) const { return NeighIter(neigh_beg(u), neigh_end(u)); }



	inline ul get_degree(const ul &u) const { return (u < n) ? cd[u+1] - cd[u] : 0; }

	void print_some(int a) const;

	void reorder(const std::vector<ul> &rank);

	Edgelist to_edgelist(bool directed=true);
	Edgelist to_edgelist_ranked(bool directed, const std::vector<ul> &rank);

  void sort_neighbours();
  bool are_neighbours_sorted();
  void soft_rank(const std::vector<ul> &rank, const std::vector<ul> &dp);

protected: // Private building operations
	void build_from_edgelist(Edgelist &g, bool sorted=false);
  void build_from_edgelist_ranked(Edgelist &g, const std::vector<ul> &rank);


    // pure virtual functions need to be implemented by inherited classes https://stackoverflow.com/a/1306837
	virtual std::vector<ul> compute_degrees(Edgelist &g) = 0;
	virtual void build_adj_array(Edgelist &g, std::vector<ul> &deg) = 0;
	virtual bool build_edge_condition(const ul &u, const ul &v) = 0;

	void compute_cumulated_degrees(std::vector<ul> &deg);
};

struct Uadjlist : Adjlist {
	Uadjlist(Edgelist &g);
  Uadjlist(Edgelist &g, const std::vector<ul> &rank);
	std::vector<ul> compute_degrees(Edgelist &g);
	void build_adj_array(Edgelist &g, std::vector<ul> &deg);
	inline bool build_edge_condition(const ul &u, const ul &v) { return u < v; }
};

struct Dadjlist : Adjlist {
  Dadjlist(Edgelist &g);
  Dadjlist(Edgelist &g, const std::vector<ul> &rank);
	std::vector<ul> compute_degrees(Edgelist &g);
	void build_adj_array(Edgelist &g, std::vector<ul> &deg);
	inline bool build_edge_condition(const ul &u, const ul &v) { return true; }
};

struct Badjlist : Adjlist {
  Badjlist(Edgelist &g);
  Badjlist(Edgelist &g, const std::vector<ul> &rank);
  std::vector<ul> compute_degrees(Edgelist &g);
  void build_adj_array(Edgelist &g, std::vector<ul> &deg);
  inline bool build_edge_condition(const ul &u, const ul &v) { return true; }

  void print_some(int a) const;

  inline ul get_degOut(const ul &u) const { return cd[u+1] - cd[u]; }
  inline ul get_degIn(const ul &u) const { return cd[u+1+n] - cd[u+n]; }
  inline ul get_deg(const ul &u) const { return get_degOut(u) + get_degIn(u); }

  inline ul* neighOut_beg(const ul &u) { return &adj[cd[u]]; }
  inline const ul* neighOut_beg(const ul &u) const { return &adj[cd[u]]; }
  inline ul* neighOut_end(const ul &u) { return &adj[cd[u+1]]; }
  inline const ul* neighOut_end(const ul &u) const { return &adj[cd[u+1]]; }

  inline ul* neighIn_beg(const ul &u) { return &adj[cd[u+n]]; }
  inline const ul* neighIn_beg(const ul &u) const { return &adj[cd[u+n]]; }
  inline ul* neighIn_end(const ul &u) { return &adj[cd[u+1+n]]; }
  inline const ul* neighIn_end(const ul &u) const { return &adj[cd[u+1+n]]; }

  inline NeighIter neighOut_iter(const ul &u) { return NeighIter(neighOut_beg(u), neighOut_end(u)); }
  inline const NeighIter neighOut_iter(const ul &u) const { return NeighIter(neighOut_beg(u), neighOut_end(u)); }
  inline NeighIter neighIn_iter(const ul &u) { return NeighIter(neighIn_beg(u), neighIn_end(u)); }
  inline const NeighIter neighIn_iter(const ul &u) const { return NeighIter(neighIn_beg(u), neighIn_end(u)); }

};

#endif
