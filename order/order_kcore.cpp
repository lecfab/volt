// See header for general documentation

#include "order_kcore.h"
#include "../utils/adjlist.h"
#include "../utils/heap.h"

using namespace std;

Kdegeneracies algo_kcore(const Adjlist &g) {
  Bheap h(g.n);
  if(g.directed) {
    Debug("Directed in-kcore")
    vector<ul> degIn(g.n, 0);
    for (ul u = 0; u < g.n; ++u)
      for (auto &v : g.neigh_iter(u))
        degIn[v] ++;
    for (ul u = 0; u < g.n; ++u)
		  h.insert(Keyvalue(u, degIn[u]));
  }
  else {
    Debug("Undirected kcore")
  	for (ul u = 0; u < g.n; ++u)
  		h.insert(Keyvalue(u, g.get_degree(u)));
  }

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = i; // g.n - i;
		if(kv.val > degeneracy)	degeneracy = kv.val;
		kd.degeneracies[u] = degeneracy;
    int count = 0;
    for (auto &v : g.neigh_iter(u)) {
      if(h.contains(v)) count ++;
      h.update_decrement(v);
    }
	}
  Info("Core value: " << degeneracy)

	return kd;
}
