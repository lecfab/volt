#ifndef ALGO_TRIANGLES_H
#define ALGO_TRIANGLES_H

#include "../utils/tools.h"
struct Edgelist;
struct Adjlist;

bool triangle_complexities(const Adjlist &g, const std::vector<ul> &rank, std::vector<ul> &dp); // computes complexities wrt given rank

ull list_triangles_bool_indep(bool printTriangles, const Adjlist &g, const std::vector<ul> &dp, std::string algo="PM");

#endif
