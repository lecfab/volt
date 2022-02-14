#include "inout.h"
#include "tools.h"
#include "edgelist.h"
#include <climits>
#include <iostream>
#include <fstream> // ofstream: write, ifstream: read, fstream: read and write from/to files

using namespace std;


ul get_number_vertices(ifstream &file) {
  ul n=0, u, v;
  while(file >> u >> v) {
    //Debug(n << ' '<<u <<' '<< v)
    n = max3(n, u, v);
  }
  return n+1;
}


vector<ul> read_order(ifstream &file) {
  vector<ul> rank; rank.reserve(NNODES);
	for(ul i=0; file >> i;) rank.push_back(i);
	return rank;
}

// Experiment: FILE* seems to be 20x faster than ofstream <<
void c_printorder(const vector<ul> &rank, const ul n, const char* output){
  FILE *file = fopen(output, "w");
  for (ul u=0; u<n; ++u) {
    if(rank[u] >= n) {
      Alert("rank " << u << " is " << rank[u])
      continue;
    }
    fprintf(file, "%" ULPRINTF "\n", rank[u]);
  }
  fclose(file);
}
