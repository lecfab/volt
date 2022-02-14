#ifndef ORDER_TRIANGLEDPP_H
#define ORDER_TRIANGLEDPP_H


#include "../utils/tools.h"

struct Adjlist;
struct ContinuousRank;

// compute current out-going degree
std::vector<ul> compute_dp(const Adjlist &g, const ContinuousRank &cRank);

// objective functions
ull sum_square_dpp(const Adjlist &g, const std::vector<ul> &vec);
ull sum_square_dpm(const Adjlist &g, const std::vector<ul> &vec);

// search for position in neighbourhood
ul best_position_dpp(const Adjlist &g, const ul &u, const std::vector<ul> &neighbours, const std::vector<ul> &dp, const ull &i0, ull &score);
ul best_position_dpm(const Adjlist &g, const ul &u, const std::vector<ul> &neighbours, const std::vector<ul> &dp, const ull &i0, ull &score);

// main optimisations
std::vector<ul> optimise_dpm_degcheck(const Adjlist &g);

std::vector<ul> place_neighbour(const Adjlist &g, decltype(sum_square_dpm) &scoring, decltype(best_position_dpp) &best_position, double precision, std::vector<ul> &order);
std::vector<ul> place_neighbour(const Adjlist &g, decltype(sum_square_dpm) &scoring, decltype(best_position_dpp) &best_position, double precision=1e-2);
std::vector<ul> place_neighbour(const Adjlist &g, decltype(sum_square_dpm) &scoring, decltype(best_position_dpp) &best_position, std::vector<ul> &order);
template<class ...Ts> std::vector<ul> place_neighbour_dpm(const Adjlist &g, Ts... args) { Info("place_neighbour_dpm") return place_neighbour(g, sum_square_dpm, best_position_dpm, args...); }
template<class ...Ts> std::vector<ul> place_neighbour_dpp(const Adjlist &g, Ts... args) { Info("place_neighbour_dpp") return place_neighbour(g, sum_square_dpp, best_position_dpp, args...); }

#endif
