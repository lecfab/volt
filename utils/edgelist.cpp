#include <algorithm>
#include <climits>

#include "edgelist.h"
#include "tools.h"

using namespace std;


Edgelist::Edgelist(ul n, intEdge e) : n(n), e(e) {
	edges.reserve(e);
}
Edgelist::Edgelist(ifstream &file) {
	edges.reserve(NLINKS);
	n=0;
	ul u, v;
	while(file >> u >> v) {
    if(u == v) {
      Alert("Self-loops are not authorised. Found one with node "<< u)
      exit(1);
    }
		edges.push_back(edge(u, v));
		n = max3(n, u, v);
	}
  if(edges.size() > MAXLINKS) {
    Alert("This file has "<< edges.size() <<" edges but the maximum is "<< MAXLINKS <<". Change MAXLINKS in tools.h to use long integers; note that it will require twice the amount of RAM.")
    exit(1);
  }
	e = edges.size();
	n++;
}

void Edgelist::sort_edges() {
  sort(edges.begin(), edges.end(), [&](const edge a, const edge b) -> bool {
    return a.first < b.first or (a.first == b.first && a.second < b.second);
  });
}

void Edgelist::apply_rank(const vector<ul> &rank) {
  // compute_degrees(true);
  for(ul i=0; i<e; i++)
    edges[i] = edge(rank[edges[i].first], rank[edges[i].second]);
  compute_degrees(true);
}

void Edgelist::to_dag() { // swap edges to obtain a directed acyclic graph
  compute_degrees();
  for(intEdge i=0; i<e; i++) {
    ul u = edges[i].first, v = edges[i].second;
    if(u > v) {
      edges[i] = edge(v, u);
      degOut[u] --; degIn[u] ++;
      degOut[v] ++; degIn[v] --;
    }
  }
}

void Edgelist::print_c(const char* output) const {
  FILE *file = fopen(output,"w");
	for (auto &edge : edges) fprintf(file, "%" ULPRINTF " %" ULPRINTF "\n", edge.first, edge.second);
	fclose(file);
}
void Edgelist::print_c_binary(const char* output) const {
  FILE *file = fopen(output,"wb");
  if(n < UINT_MAX) {
    Info("Using unsigned int instead of unsigned long because n<"<< UINT_MAX)
    vector<ul> compressed_edges; compressed_edges.reserve(2*e);
    for (auto &edge : edges) {
      compressed_edges.push_back(edge.first);
      compressed_edges.push_back(edge.second);
    }
    fwrite(&compressed_edges[0], sizeof(compressed_edges[0]), 2*e, file);
  }
  else {
    Info("Using unsigned long because n>"<< UINT_MAX)
    vector<ul> compressed_edges; compressed_edges.reserve(2*e);
    for (auto &edge : edges) {
      compressed_edges.push_back(edge.first);
      compressed_edges.push_back(edge.second);
    }
    fwrite(&compressed_edges[0], sizeof(compressed_edges[0]), 2*e, file);
  }
  fclose(file);
}


ul Edgelist::get_degIn(const ul &u) {
  if(!deg_computed)
    compute_degrees();
  return degIn[u];
}
ul Edgelist::get_degOut(const ul &u) {
  if(!deg_computed)
    compute_degrees();
  return degOut[u];
}
ul Edgelist::get_deg(const ul &u) {
  if(!deg_computed)
    compute_degrees();
  return degIn[u] + degOut[u];
}

void Edgelist::print_some(intEdge a) const {
  if(a>e) a=e;
  cout << "# printing " << a << " edges" << endl;
  for(intEdge i=0; i<a; ++i)
    cout <<"\t"<< edges[i].first << " " << edges[i].second << endl;
}

void Edgelist::compute_degrees(bool force) {
  if(deg_computed and !force) return;
  degOut = vector<ul>(n, 0);
  degIn = vector<ul>(n, 0);
	for (auto &edge : edges) {
    ++degOut[edge.first];
    ++degIn[edge.second];
  }

  deg.reserve(n);
  for (ul u = 0; u < n; ++u) {
    deg[u] = degIn[u] + degOut[u];
  }

  deg_computed = true;
}
