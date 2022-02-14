#ifndef CONTINUOUSRANK_H
#define CONTINUOUSRANK_H

// Structure to store a ranking of n nodes, with the following operations:
// - move a node before or after any other node in O(1) amortised
// - compare the ranks of two nodes in O(1)
// - output the (integer) ranking or ordering in O(n)

// It uses a continuous value "altitude". When a node is inserted between two
// nodes, it takes the average value. If the precision reaches floating point
// limits, then altitude is normalised in O(n)

#include "tools.h"

struct ContinuousRank {
public:
  ul n;                         // number of nodes
  std::vector<double> altitude; // floating rank value of each node
  std::list<ul> chain;          // linked list to acces next and prev elements of the rank
  std::vector<std::list<ul>::iterator> pointer; // pointer for each node into the chain

  ContinuousRank(ul n); // create the rank from natural ordering
  ContinuousRank(ul n, std::vector<ul> &order); // create the rank from a given order
  bool check_sanity();

  void move(ul &u, std::list<ul>::iterator pointer_next);
  void move_before(ul &u, ul &v);
  void move_after(ul &u, ul &v);

  std::vector<ul> to_rank();
  std::vector<ul> to_order();

  ul normalisations = 0;
  void normalise();

  // bool operator()(const ul &u, const ul &v) { return altitude[u] < altitude[v] or (altitude[u] == altitude[v] and u < v); }

        double& operator[](ul u)       { return altitude[u]; }
  const double& operator[](ul u) const { return altitude[u]; }
};

#endif
