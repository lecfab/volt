#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include "tools.h"

struct Keyvalue;


struct Bheap {
  ul n_max;
  ul n;
  std::vector<ul> pt;
  std::vector<Keyvalue> kv;

  Bheap(ul n_max);

  void insert(Keyvalue new_kv);
  void update(ul key, ul new_val);
  void update_decrement(ul key);
  void update_increment(ul key);
  void remove(ul key);
  bool contains(ul key);

  Keyvalue popmin();
  Keyvalue getmin();

  void binary_swap(ul i, ul j);
  ul bubble_up(ul child);
  ul bubble_down(ul parent);
  ul key_to_val(ul key);
};

#endif
