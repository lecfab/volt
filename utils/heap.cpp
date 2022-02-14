#include <vector>

#include "heap.h"
#include "tools.h"

using namespace std;

Bheap::Bheap(ul n_max) : n_max(n_max) {
	n = 0;
	pt.reserve(n_max);
	kv.reserve(n_max);
}

void Bheap::binary_swap(ul i, ul j) {
	std::swap(pt[ kv[i].key ], pt[ kv[j].key ]);
	std::swap(kv[i], kv[j]);
}

ul Bheap::bubble_up(ul child) {
	while (child > 0) {
		ul parent = (child-1) / 2;
		if (kv[parent].val <= kv[child].val) break;
		binary_swap(child, parent);
		child = parent;
	}
  return child;
}

ul Bheap::bubble_down(ul parent) {
	while(2*parent + 1 < n) {
    ul left = 2*parent + 1, right = left + 1;
    ul child = left;
		if(right < n and kv[right].val < kv[left].val)
			 child = right;

		if(kv[parent].val <= kv[child].val) break;

		binary_swap(parent, child);
		parent = child;
	}
  return parent;
}

void Bheap::insert(Keyvalue new_kv) {
	kv.push_back(new_kv);
	pt[new_kv.key] = n;
	bubble_up(n);
  n++;
}

ul Bheap::key_to_val(ul key) {
	return kv[pt[key]].val;
}

void Bheap::update_decrement(ul key){
	if (!contains(key)) return;
  ul i = pt[key];
	kv[i].val --;
	bubble_up(i);
}
void Bheap::update_increment(ul key){
	if (!contains(key)) return;
  ul i = pt[key];
	kv[i].val ++;
	bubble_down(i);
}

void Bheap::update(ul key, ul new_val){
	if (!contains(key)) return;
  ul i = pt[key];
	kv[i].val = new_val;
	bubble_down(bubble_up(i));
}

Keyvalue Bheap::popmin() {
	Keyvalue min = kv[0];
	remove(min.key);
	return min;
}
Keyvalue Bheap::getmin() {
	return kv[0];
}

void Bheap::remove(ul key) {
  ul i = pt[key];
  kv[i] = kv[--n];
  pt[kv[i].key] = i;
  pt[key] = n_max + 1;
	bubble_down(i);
}

bool Bheap::contains(ul key) {
  return pt[key] <= n_max;
}
