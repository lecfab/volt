#ifndef ALGO_KCORE_H
#define ALGO_KCORE_H

// K-core decomposition (https://en.wikipedia.org/wiki/Degeneracy_(graph_theory)#Algorithms)

// Remove the node of lowest degree, then update the graph. Repeat.
// Gives a ranking of nodes according to their coreness or degeneracy.

// Method: use a binary heap to keep track of nodes' updated degrees.

// Variants:
// * apply it on a directed graph
// * count only in- or out-going edges
// * remove hubs (high-degree nodes) instead of low-degree nodes

// Space optimisation:
// * remove rank if you only need degeneracy
// * remove degeneracy if you only need rank
// * compact the degeneracy array by remembering only the indices where degeneracy increases

#include "../utils/tools.h"
#include <vector>
struct Adjlist;
struct Bheap;


// Structure used to return both rank and degeneracy arrays
struct Kdegeneracies {
  ul n;
  std::vector<ul> degeneracies;
  std::vector<ul> rank;
  Kdegeneracies(ul n) : n(n) {
    degeneracies.reserve(n);
    rank.reserve(n);
  }
};

Kdegeneracies algo_kcore(const Adjlist &g); // standard version

#endif
