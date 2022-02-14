#ifndef INOUT_H
#define INOUT_H

#include <fstream> // ofstream: write, ifstream: read, fstream: read and write from/to files
#include "tools.h"



ul get_number_vertices(std::ifstream &file);

std::vector<ul> read_order(std::ifstream &file);
// Experiment: FILE* seems to be 20x faster than ofstream <<
void c_printorder(const std::vector<ul> &rank, const ul n, const char* output);
void c_printorder_binary(const std::vector<ul> &rank, const ul n, const char* output);


#endif
