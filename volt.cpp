#include <iostream>
#include <iomanip>
#include <string>

#include <vector>
#include <cmath>

#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <omp.h>        // parallelisation


#include "utils/CLI11.h" // options parser
#include "utils/tools.h"
#include "utils/inout.h"
#include "utils/adjlist.h"
#include "utils/edgelist.h"
#include "utils/continuousrank.h"
#include "order/order_rand.h"
#include "order/order_deg.h"
#include "order/order_triangles.h"
#include "order/order_kcore.h"
#include "algo/algo_triangles.h"

using namespace std;


int main(int argc, char** argv) {
  // Info("Max possible nodes: " << MAXNODES << "; edges: " << MAXLINKS <<"; max unsigned int: "<< UINT_MAX <<"; type %" ULPRINTF)
	TimeBegin()

  CLI::App app{"Vertex Orderings to List Triangles: apply an ordering on a list of edges to list all triangles faster. See README.md for more information."};

  string filename, ordering="check", algorithm="PM", output_edges="null";
  double number = 1e-2; int parallel_threads = 1; bool output_triangles = false;
  app.add_option("dataset", filename, "Text file: list of `a b` edges with nodes IDs ranging from 0 to N-1")->required();
  app.add_option("-o,--order", ordering, "Ordering used to relabel the nodes")//->required()
    ->check(CLI::IsMember({
      "original", "rand","deg", "core", "split", "check","neigh","neighDpp","neighDpm"}, CLI::ignore_case))->capture_default_str();
  app.add_option("-a,--algo", algorithm, "Algorithm to execute on the reordered graph")
    ->check(CLI::IsMember({"comparison", "PM", "PP"}, CLI::ignore_case))->capture_default_str();
  app.add_option("-e,--edges", output_edges, "File in which to output edges with new indices")->capture_default_str();
  app.add_flag("-t,--triangles", output_triangles, "Print triangles in standard output (add >f to redirect into file f)")->capture_default_str();
  app.add_option("-n", number, "Numeric parameter used in different orders")->capture_default_str();
  app.add_option("-p", parallel_threads, "Number of parallel threads")->capture_default_str();

  CLI11_PARSE(app, argc, argv);

	ifstream file(filename);
  ul n = 0;
  vector<ul> rank;
  Edgelist* h = NULL;
  Adjlist* g = NULL;
  srand ( unsigned ( time(0) ) ); // initialise randomness
  omp_set_num_threads(parallel_threads);
  if(parallel_threads > 1) { Info("Running "<< parallel_threads <<" parallel threads") }


	// --------------------------------------------------
	// -------- Store the graph in an edgelist ----------
	// --------------------------------------------------
	Info("Reading edgelist from file " << filename)
	h = new Edgelist(file); //*h = c_readedgelist(argv[1]);
	n = h->n;
  rank.reserve(n);

	Info("Number of nodes: " << h->n)
	Info("Number of edges: " << h->e)
	TimeStep("Read")

  if(ordering == "original" or ordering == "rand") {
    rank = order_identity(n);

		if(ordering == "rand")
    	random_shuffle( rank.begin(), rank.end() );
	}
  else if(ordering == "degDesc") rank = order_deg(*h, /*desc=*/true);
  else if(ordering == "deg" or ordering == "degAsc") rank = order_deg(*h, /*desc=*/false);
  else if(ordering == "split") rank = order_deg_split(*h);

	// --------------------------------------------------
	// -- Convert edgelist file into Adjlist structure --
	// --------------------------------------------------
  else {
  	Info("Converting to adjacency list");
    g = new Uadjlist(*h);
  	TimeStep("Adjlist")

    if(ordering == "core") rank = algo_kcore(*g).rank;
    else if(ordering == "neighDpp") rank = place_neighbour_dpp(*g, number);
    else if(ordering == "neighDpm" or ordering == "neigh") rank = place_neighbour_dpm(*g, number);
    else if(ordering == "check") rank = optimise_dpm_degcheck(*g);
  	else { Alert("Unknown order `" << ordering <<"`"); return 1; }
    // delete g;
  }

	TimeStep("Order")

  // --------------------------------------------------
  // -------- Export edges with new indices -----------
  // --------------------------------------------------
  if(output_edges != "null") {
    Info("Export edges with new indices into file " << output_edges)
    h->apply_rank(rank);
    h->to_dag();
    h->sort_edges();
    h->print_c(output_edges.c_str());
    TimeStep("Export")
  }

  // --------------------------------------------------
  // ---------- Execute triangle algorithm ------------
  // --------------------------------------------------
  if(algorithm == "comparison" or algorithm == "PM" or algorithm == "PP") {
    // ---------- Version with fast transform ----------
    if(g == NULL) {
      Info("Converting to adjacency list");
      g = new Uadjlist(*h);
    }
    // vector<ul> dp; dp.reserve(g->n);
    vector<ul> dp(g->n, 0);
    bool dpp_is_better = triangle_complexities(*g, rank, dp);

    g->soft_rank(rank, dp); // put out-neighbours at the beginning of adjacency
    TimeStep("Transform")

    if(algorithm == "comparison" or algorithm == "PP") {
      if(dpp_is_better)
        list_triangles_bool_indep(output_triangles, *g, dp, "PP");
      else
        list_triangles_bool_indep(output_triangles, *g, dp, "MM");
      TimeStep("PP")
    }
    if(algorithm == "comparison" or algorithm == "PM") {
      list_triangles_bool_indep(output_triangles, *g, dp, "PM");
      TimeStep("PM")
    }
    delete g;
  }
  if(h != NULL) { delete h; }

  TimeTotal()
	return 0;
}
