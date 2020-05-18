/*********************************************
 * file:	~\tnd004\lab\lab4b\dsets.cpp         *
 * remark: implementation of disjoint sets    *
 * 
 * Authors:
 * Victor Lindquist, vicli268
 * Johan Linder, johli153
 **********************************************/

#include <iostream>
#include <iomanip>
#include <cassert>

//using namespace std;

#include "dsets.h"

// -- CONSTRUCTORS

DSets::DSets(int theSize) {
	assert(theSize > 0);
	size = theSize;
	array = new int[size + 1];
	init();
}

// -- DESTRUCTOR

DSets::~DSets() {
	delete[] array;
	array = nullptr;
	size = 0;
}

// -- MEMBER FUNCTIONS

// create initial sets
void DSets::init() {
	for (int i = 1; i <= size; i++) {
		array[i] = -1;  // every disjoint set has one node -- singleton
	}
}

// join sets named r and s where r != s
// i.e. join trees with roots r and s
void DSets::join(int r, int s) {
	assert(r != s);
	assert(r >= 1 && r <= size);
	assert(s >= 1 && s <= size);
	assert(array[r] < 0);
	assert(array[s] < 0);

	// simple union
	// array[r] = s;

	// *** TODO *** Done
	// Union by size
	if(array[r] > array[s]){
		array[s] += array[r];
		array[r] = s;
	}
	else {
		array[r] += array[s];
		array[s] = r;
	}
}

// return name of current set for x
// i.e. return root of tree for x
int DSets::find(int x) {
	assert(x >= 1 && x <= size);

	// simple find
	// if (array[x] < 0) {
	// 	return x;
	// } else {
	// 	return find(array[x]);
	// }

	// *** TODO *** Done
	// find with path compression
	if (array[x] < 0) {
		return x;
	} else {
		//Find root and update paths
		return array[x] = find(array[x]);
	}
}

// just in case ...
void DSets::print() const {
	std::cout << "\n";

	for (int i = 1; i <= size; i++) {
		std::cout << std::setw(4) << i;
	}

	std::cout << "\n";

	for (int i = 1; i <= size; i++) {
		std::cout << std::setw(4) << array[i];
	}

	std::cout << "\n";
}
