#include "order_rand.h"
#include "../utils/tools.h"
#include "../utils/adjlist.h"

#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

using namespace std;


vector<ul> order_rand(Adjlist &g) {
	Debug("Order rand")
  vector<ul> rank = order_identity(g.n);
  srand ( unsigned ( time(0) ) );
  random_shuffle( rank.begin(), rank.end() );
  // auto rd = random_device {};
  // auto rng = default_random_engine { rd() };
  // shuffle(rank.begin(), rank.end(), rng);

  return rank;
}
