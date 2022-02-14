// See header for general documentation

#include "continuousrank.h"

using namespace std;


ContinuousRank::ContinuousRank(ul n) : n(n) {
  altitude.reserve(n);
  pointer.reserve(n);
  for(ul u=0; u<n; ++u)
    pointer.push_back(chain.insert(chain.end(), u));
  normalise();
}

ContinuousRank::ContinuousRank(ul n, vector<ul> &order) : n(n) {
  altitude.reserve(n);
  pointer.reserve(n);
  for(auto &u : order) {
    pointer[u] = chain.insert(chain.end(), u);
  }
  normalise();
}

bool ContinuousRank::check_sanity() {
  double alt_min = -n;
  for(auto &u : chain) {
    if(alt_min >= altitude[u]) {
      Alert("Error: node "<< u <<" has altitude "<< altitude[u] <<" lower than minimum "<<alt_min)
      exit(0);
    }
    alt_min = altitude[0];
  }
  return true;
}

void ContinuousRank::normalise() { // could we normalise only where it is useful?
  normalisations ++;
  // Info("Normalising altitude")
  double counter = -((double) n) / 2;
  for(auto &u : chain) altitude[u] = counter++;
}

void ContinuousRank::move_after(ul &u, ul &v)  { move(u, next(pointer[v])); }
void ContinuousRank::move_before(ul &u, ul &v) { move(u, pointer[v]); }
void ContinuousRank::move(ul &u, list<ul>::iterator pointer_next) {
  chain.erase(pointer[u]);
  double altitude_prev = altitude[chain.front()] - 1;
  double altitude_next = altitude[chain.back()] + 1;
  if(pointer_next != chain.end()) altitude_next = altitude[*pointer_next];
  if(pointer_next != chain.begin()) altitude_prev = altitude[*prev(pointer_next)];
  pointer[u] = chain.insert(pointer_next, u);

  altitude[u] = (altitude_next + altitude_prev) / 2;
  if(altitude_prev >= altitude[u] or altitude[u] >= altitude_next) { // small float limit
    // Info(altitude_prev <<" "<< altitude[u] <<" "<< altitude_next)
    normalise();
  }
}

vector<ul> ContinuousRank::to_rank() {
  vector<ul> rank; rank.reserve(n);
  ul counter = 0;
  for(auto &u : chain) rank[u] = counter++;
  return rank;
}

vector<ul> ContinuousRank::to_order() {
  vector<ul> order; order.reserve(n);
  for(auto &u : chain) order.push_back(u);
  return order;
}
