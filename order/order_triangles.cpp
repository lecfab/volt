// See header for general documentation

#include <functional>
#include <unordered_set>
#include <set>
#include "order_triangles.h"
#include "../utils/adjlist.h"
#include "../utils/continuousrank.h"
#include "../utils/heap.h"

using namespace std;


bool tmp__compare_nodedeg_desc (Keyvalue &a, Keyvalue &b) { // sort degree DESC, node ASC
	return ( a.val > b.val or (a.val == b.val and a.key < b.key) );
}
vector<ul> tmp__order_deg_desc(const Adjlist &g) {
  vector<Keyvalue> toSort; toSort.reserve(g.n);
  for (ul u = 0; u < g.n; ++u) toSort.push_back( Keyvalue(u, g.get_degree(u)) );
  sort(toSort.begin(), toSort.end(), tmp__compare_nodedeg_desc);
  vector<ul> order; order.reserve(g.n);
  for (ul i = 0; i < g.n; ++i) order[i] = toSort[i].key;
  return order;
}

vector<ul> optimise_dpm_degcheck(const Adjlist &g) {
  Info("optimise_dpm_degcheck")

  vector<ul> order = tmp__order_deg_desc(g);
  vector<ul> current_dp(g.n, 0);
  vector<ul> current_dm(g.n, 0);
  vector<ul> rank; rank.reserve(g.n);

  ul i = 0, j = g.n - 1;
  while(i <= j) {
    ul u = order[i + g.n - 1 - j];
    if(current_dp[u] >= current_dm[u]) {
      rank[u] = i++; // u stays as early as possible in the ordering
      current_dp[u] = g.get_degree(u) - current_dm[u];
      for (auto &v : g.neigh_iter(u))// v has one more in-going neighbour
        if(current_dp[v] + current_dm[v] < g.get_degree(v))
          current_dm[v]++;
    }
    else {
      rank[u] = j--; // u goes as late as possible in the ordering
      current_dm[u] = g.get_degree(u) - current_dp[u];
      for (auto &v : g.neigh_iter(u)) // v has one more out-going neighbour
        if(current_dp[v] + current_dm[v] < g.get_degree(v))
          current_dp[v]++;
    }
	}

  // ull score = sum_square_dpm(g, current_dm); // to display the final score
  // intEdge m = g.e / g.edge_factor;
  // cout << "Final dpm/edge = " << ((double) score) / m << endl;

	return rank;
}




// mistakes: pi should have double type in neighbour_positions
// mistakes: end value should be pi+1 and not pointer+1
// mistakes: unsigned and long are bad together. https://stackoverflow.com/questions/70583170/substracting-long-int-and-unsigned-int
// improved: watchlist of updated nodes
// improved: linear computing of best penalties
// improved: linked list instead of array where everyone had to be moved one by one
// improved: when u goes after v, put u close to v instead of at the end of the chain (normalisation and cache purpose)
// improved: ContinuousRank is now a separate structure with hidden tools
// improved: precision can be loop threshold (0 ≤ epsilon < 1) or number of iterations (≥ 1)
// tried: watch heap instead of bool-array, but it seems slower in practice
// todo: altitude to be evened out among several predecessors
// todo: move to closest neighbour even when i0 is unchanged; this could increase normalisations
vector<ul> place_neighbour(const Adjlist &g, decltype(sum_square_dpm) &scoring, decltype(best_position_dpp) &best_position, double precision) { vector<ul> order{}; return place_neighbour(g, scoring, best_position, precision, order); }
vector<ul> place_neighbour(const Adjlist &g, decltype(sum_square_dpm) &scoring, decltype(best_position_dpp) &best_position, vector<ul> &order) { return place_neighbour(g, scoring, best_position, 1e-2, order); }
vector<ul> place_neighbour(const Adjlist &g, decltype(sum_square_dpm) &scoring, decltype(best_position_dpp) &best_position, double precision, vector<ul> &order) {
  intEdge m = g.e / g.edge_factor;
  // ----- preparation -----
  ContinuousRank cRank = order.empty() ? ContinuousRank(g.n) : ContinuousRank(g.n, order);
  vector<ul> dp = compute_dp(g, cRank);
  vector<bool> watch(g.n, true);

  // ----- loop until stability is reached -----
  ull score = scoring(g, dp), previous_score = score * 2;
  ull initial_score = score;
  cout << "Initial score: " << ((double) score) / m << endl;
  int iteration = 0;
  while(score < previous_score) {
    if(precision >= 1) {
      if(iteration >= precision) {
        Info("Stopped after iteration "<< iteration)
        break;
      }
    }
    else if(score >= (1.-precision) * previous_score) {
      Info("Truncated with precision "<< precision)
      break;
    }
    previous_score = score;

    // select nodes that have been updated
    vector<ul> nodes; nodes.reserve(g.n);
    for(ul u=0; u<g.n; ++u)
      if(watch[u]) {
        watch[u] = false;
        nodes.push_back(u);
      }

    // ----- consider each node in random order -----
    random_shuffle(nodes.begin(), nodes.end());
    for(auto &u : nodes) {
      // if(g.get_degree(u) == 0) continue;
      vector<ul> neighbours; neighbours.reserve(g.get_degree(u));
      for(auto &v : g.neigh_iter(u)) neighbours.push_back(v);
      sort(neighbours.begin(), neighbours.end(), [&cRank](ul v, ul w) { return cRank[v] < cRank[w]; });
      // ----- find the best position in its neighbourhood -----
      ul i0 = g.get_degree(u) - dp[u];
      ul new_pos = best_position(g, u, neighbours, dp, i0, score); // updates score
      if(new_pos == i0) continue;

      if(score == 0 or score >= initial_score) {
        Alert("Score is "<<score<<" after changing "<< u <<" from position "<<i0<< " to "<<new_pos<<" / "<< neighbours.size())
        exit(1);
      }

      // ----- put it in the best position -----
      if(new_pos == g.get_degree(u)) cRank.move_after(u, neighbours.back());
      else cRank.move_before(u, neighbours[new_pos]);

      // ----- update out-degrees -----
      dp[u] = g.get_degree(u) - new_pos;
      for(ul j=new_pos; j<i0; ++j) dp[ neighbours[j] ] --;
      for(ul j=i0; j<new_pos; ++j) dp[ neighbours[j] ] ++;

      // for(auto &v : neighbours) { Info("\t"<< cRank[v])}
      for(auto &v : neighbours) watch[v] = true;
    }

    cout << "Iter " << ++iteration
         << " \tScore: " << ((double) score) / m
         << " \tNodes %: "<< 100*((double) nodes.size()) / g.n
         << " \tNormalisations: " << cRank.normalisations << endl;
  }

  // cout << "recompute score" << endl;
  // dp = compute_dp(g, cRank); // just to check that score is accurate
  // score = scoring(g, dp);
  cout << "Final score:   " << ((double) score) / m << endl;

  return cRank.to_rank();
}

vector<ul> compute_dp(const Adjlist &g, const ContinuousRank &cRank) {
  vector<ul> dp(g.n, 0);
  for(ul u=0; u<g.n; ++u)
    for(auto &v : g.neigh_iter(u))
      if(cRank[v] > cRank[u]) dp[u] ++;
  return dp;
}

ull sum_square_dpp(const Adjlist &g, const vector<ul> &vec) {
  ull s = 0;
  for(ull a : vec)
    s += pow(a, 2);
  return s;
}
ull sum_square_dpm(const Adjlist &g, const vector<ul> &dp) {
  ull s = 0;
  for(ul u=0; u<g.n; ++u)
    s += (ull) dp[u] * (g.get_degree(u) - dp[u]);
  return s;
}


ul best_position_dpm(const Adjlist &g, const ul &u, const vector<ul> &neighbours, const vector<ul> &dp, const ull &i0, ull &score) {
  ull curr_score = score;
  ull best_score = score;
  ull new_pos = i0;

  for(ull i=i0; i>0; --i) {
    ull j = i-1;
    curr_score = curr_score + 2*(ull)dp[neighbours[j]] - (ull)g.get_degree(neighbours[j]) - 1; // changed contribution of swapped neighbour
    ull new_score = curr_score - (long long int)(i0-j) * (long long int)(g.get_degree(u)-i0-j); // new contribution of u
    if(new_score < best_score) { best_score = new_score; new_pos = j; }
  }
  curr_score = score;
  for(ull i=i0; i<g.get_degree(u); ++i) {
    ull j = i+1;
    curr_score = curr_score + (ull)g.get_degree(neighbours[i]) - 2*(ull)dp[neighbours[i]] - 1;
    ull new_score = curr_score + (long long int)(j-i0) * (long long int)(g.get_degree(u)-i0-j);
    if(new_score < best_score) { best_score = new_score; new_pos = j; }
  }

  score = best_score;
  return new_pos;
}



ul best_position_dpp(const Adjlist &g, const ul &u, const vector<ul> &neighbours, const vector<ul> &dp, const ull &i0, ull &score) {
  ull curr_score = score;
  ull best_score = score;
  ull new_pos = i0;

  for(ull i=i0; i>0; --i) {
    ull j = i-1;
    curr_score = curr_score + 1 - 2*(ull)dp[neighbours[j]]; // changed contribution of swapped neighbour
    ull new_score = curr_score + (i0-j) * (2*g.get_degree(u)-i0-j); // new contribution of u
    if(new_score < best_score) { best_score = new_score; new_pos = j; }
  }
  curr_score = score;
  for(ull i=i0; i<g.get_degree(u); ++i) {
    ull j = i+1;
    curr_score = curr_score + 1 + 2*(ull)dp[neighbours[i]];
    ull new_score = curr_score - (j-i0) * (2*g.get_degree(u)-i0-j);
    if(new_score < best_score) { best_score = new_score; new_pos = j; }
  }

  score = best_score;
  return new_pos;
}
