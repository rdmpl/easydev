//
// This file is used for calucatint how many
// tables does a lot of elements belongs
// using the randomized algorithm to reduce
// time and space complexity.
//
#ifndef GATHER_GATHER_H_
#define GATHER_GATHER_H_
#include <vector>
class Gather{
	std::vector<int> f;
	public:
	Gather(int n);
	int find(int v);
	void merge(int a, int b);
};
#endif
