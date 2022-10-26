// VOLT: Vertex Ordering to List Triangles
// Copyright (C) 2022 Fabrice Lécuyer (fabrice.lecuyer@lip6.fr)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 or later.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details: https://www.gnu.org/licenses/

#include <iostream>
#include <iomanip>
#include <string>

#include <vector>
#include <cmath>
#include <algorithm>

#include "utils/tools.h"

using namespace std;

// inline ul max3(ul a, ul b, ul c) { return (a >= b and a >= c) ? a : ((b >= c) ? b : c); /}

int main(int argc, char** argv){
	TimeBegin()
  if(argc <= 2) { // secret arg added to allow for directed cleansing
    cout << "./undirect INPUT OUTPUT" << endl;
    cout << "with INPUT the initial edgelist, OUTPUT the file to output the trimmed edgelist" << endl;
    cout << "returns a (sorted) list of non-loop non-redundant edges (if both `a b` and `b a` existed, only `a b` is kept)" << endl;
    return 1;
  }
  cout << "Read file " << argv[1] << endl;
  ifstream filein(argv[1]);

  vector<pair<ul,ul>> edges;
	edges.reserve(NLINKS);
	ul n=0, u, v;
  intEdge e = 0;
  if(argc > 3) Info("Directed graph cleansing")
	while(filein >> u >> v) {
    e ++;
    if(u == v) continue;
    if(argc == 3 and u > v) swap(u, v);
		edges.push_back(make_pair(u, v));
		n = max3(n, u, v);
    // cout << "read " << u << " " << v << endl;
	}

	n++;

  TimeStep("Read")

  cout << "Number of nodes: " << n << endl;
  cout << "Number of edges: " << e << endl;
  cout << "Number of loops: " << (e - edges.size()) << endl;
  cout << endl;
  cout << "Sorting " << edges.size() << " edges" << endl;

  sort(edges.begin(), edges.end());
  TimeStep("Sort")

  cout << "Write file " << argv[2] << endl;
  FILE *fileout = fopen(argv[2], "w");
  u = v = n + 2;
  intEdge e2 = 0;
  for (auto &edge : edges) {
    if(edge.second == v and edge.first == u) {
      // Debug(u<<" " << v)
      continue;
    }
    e2 ++;
    u = edge.first;
    v = edge.second;
    fprintf(fileout, "%" ULPRINTF " %" ULPRINTF "\n", u, v);
    // cout << "written " << u << " " << v << endl;
  }
	fclose(fileout);

  cout << "Output " << e2 << " unique edges" << endl;
  TimeStep("Out")

  TimeTotal()
	return 0;
}
