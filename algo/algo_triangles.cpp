// See header for general documentation

#include <functional>
#include <unordered_set>
#include <set>
#include "algo_triangles.h"
#include "../utils/adjlist.h"
#include "../utils/edgelist.h"
#include <iomanip>
#include <omp.h>

using namespace std;

bool triangle_complexities(const Adjlist &g, const vector<ul> &rank, vector<ul> &dp) {
  ull dpp = 0, dpm = 0, dmm = 0, dep = 0;
  #pragma omp parallel for schedule(auto) reduction(+ : dpp, dpm, dmm, dep)
  for(ul u=0; u<g.n; ++u) {
    dp[u] = 0;
    for(auto &v : g.neigh_iter(u))
      if(rank[v] > rank[u]) dp[u]++;
    ull dm_u = g.get_degree(u) - dp[u];
    dpp += (ull) dp[u] * dp[u];
    dpm += (ull) dp[u] * dm_u;
    dmm += (ull) dm_u * dm_u;
    dep += (ull) g.get_degree(u) * dp[u];
  }

  double m = g.e / g.edge_factor;
  Info("O(sum d*d+) : "<< dep << " \t| per edge: " << ((double) dep)/m)
  Info("O(sum d+²) :  "<< dpp << " \t| per edge: " << ((double) dpp)/m)
  Info("O(sum d-²) :  "<< dmm << " \t| per edge: " << ((double) dmm)/m)
  Info("O(sum d+d-) : "<< dpm << " \t| per edge: " << ((double) dpm)/m)
  return (dpp < dmm);
}


ull list_triangles_bool_indep(bool printTriangles, const Adjlist &g, const vector<ul> &dp, string algo) {
  bool uOut_iter = true, vOut_iter = true; // PM algorithm
  if(algo == "PP") uOut_iter = false;      // PP algorithm
  else if(algo == "MM") vOut_iter = false; // MM algorithm
  else algo = "PM";
  Info("Listing triangles with algorithm " << algo)

  ull t = 0, c = 0;
  #pragma omp parallel reduction(+ : t, c)
  {
    vector<bool> is_neighOut(g.n, false); // use boolean array with reset
    #pragma omp for schedule(dynamic, 1)
    for(ul u=0; u<g.n; ++u) {
      for(ul i=g.cd[u]; i < g.cd[u] + dp[u]; ++i) is_neighOut[ g.adj[i] ] = true; // set array

      for(ul i=g.cd[u] + dp[u]*(!uOut_iter); i < g.cd[u+1*(!uOut_iter)] + dp[u]*uOut_iter; ++i) {
        ul v = g.adj[i];
        for(ul j=g.cd[v] + dp[v]*(!vOut_iter); j < g.cd[v+1*(!vOut_iter)] + dp[v]*vOut_iter; ++j) {
          ul w = g.adj[j];
          if(is_neighOut[w]) {
            t ++;
            if(printTriangles) {
              #pragma omp critical
              fprintf(stdout, "%" ULPRINTF " %" ULPRINTF " %" ULPRINTF "\n", u, v, w);
            }
          }
          c ++;
        }
      }

      for(ul i=g.cd[u]; i < g.cd[u] + dp[u]; ++i) is_neighOut[ g.adj[i] ] = false; // reset array
    }
  }

  double m = g.e / g.edge_factor;
  Info("Operations: "<< c << " \t| per edge: " << ((double) c) / m)
  Info("Triangles:  "<< t << " \t| per edge: " << ((double) t) / m)
  return t;
}
