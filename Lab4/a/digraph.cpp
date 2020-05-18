/*********************************************
 * file:	~\tnd004\lab\lab4a\digraph.cpp       *
 * remark: implementation of directed graphs  *
 * 
 * Authors:
 * Victor Lindquist, vicli268
 * Johan Linder, johli153
 **********************************************/

#include <iostream>
#include <iomanip>
#include <cassert>
#include <limits>
#include <string>

// using namespace std;

#include "digraph.h"
#include "queue.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

Digraph::Digraph(int n) : size{n}, n_edges{0} {
	assert(n >= 1);

	table.resize(size + 1);
	dist.resize(size + 1);
	path.resize(size + 1);
	done.resize(size + 1);
}

// Create a digraph with n vertices and the edges in V
Digraph::Digraph(const std::vector<std::tuple<int, int, int>> &V, int n) : Digraph{n} {
	for (auto [u, v, w] : V)  // C++17
	{
		insertEdge(u, v, w);
	}
}

// -- MEMBER FUNCTIONS

// insert directed edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Digraph::insertEdge(int u, int v, int w) {
	assert(u >= 1 && u <= size);
	assert(v >= 1 && v <= size);
	table[u].insert(v, w);

	n_edges += 1;
}

// remove directed edge (u, v)
void Digraph::removeEdge(int u, int v) {
	assert(u >= 1 && u <= size);
	assert(v >= 1 && v <= size);
	table[u].remove(v);

	n_edges -= 1;
}

// unweighted single source shortest paths
void Digraph::uwsssp(int s) const {
	if (s < 1 || s > size) {
		std::cout << "\nERROR: expected source s in range 1.." << size << " !\n";
		return;
	}

	// *** TODO *** Done
	Queue<int> Q{ size };
	const int MAX = std::numeric_limits<int>::max();
	
	for(int i = 1; i <= size; ++i) {
		dist[i] = MAX;
		path[i] = 0;
	}

	dist[s] = 0;
	Q.enqueue(s);
	
	while(!Q.isEmpty()) {
		int v = Q.getFront();
		Q.dequeue();

		Node* u = table[v].getFirst();

		while(u != nullptr) {
			if(dist[u->vertex] == MAX) {
				dist[u->vertex] = dist[v] + 1;
				path[u->vertex] = v;
				Q.enqueue(u->vertex);
			}

			u = table[v].getNext();
		}
	}
}

// positive weighted single source shortest paths
// Dijktra�s algorithm
void Digraph::pwsssp(int s) const {
	if (s < 1 || s > size) {
		std::cout << "\nERROR: expected source s in range 1.." << size << " !\n";
		return;
	}

	// *** TODO *** Done
	const int MAX = std::numeric_limits<int>::max();
	
	for(int i = 1; i <= size; ++i) {
		dist[i] = MAX;
		path[i] = 0;
		done[i] = false;
	}

	dist[s] = 0;
	done[s] = true;
	
	while(true) {
		Node* u = table[s].getFirst();

		while(u != nullptr) {
			if(done[u->vertex] == false && dist[u->vertex] > dist[s] + u->weight) {
				dist[u->vertex] = dist[s] + u->weight;
				path[u->vertex] = s;
			}

			u = table[s].getNext();
		}

		// Initialize min value 
		int min = MAX, minIndex;
	
		//Find min value
		for(int i = 1; i < size+1; ++i) {
			if(done[i] == false && dist[i] <= min) {
				min = dist[i];
				minIndex = i;
			}
		}
		
		//if every vertex is visited, all done.
		if(min == MAX) break;

		done[s] = true;
		s = minIndex;
	}
}

// print graph
void Digraph::printGraph() const {
	std::cout << "------------------------------------------------------------------\n";
	std::cout << "vertex  adjacency list                  \n";
	std::cout << "------------------------------------------------------------------\n";

	for (int v = 1; v <= size; v++) {
		std::cout << setw(4) << v << " : ";
		table[v].print();
	}

	std::cout << "------------------------------------------------------------------\n";
}

// print shortest path tree for s
void Digraph::printTree() const {
	std::cout << "----------------------\n";
	std::cout << "vertex    dist    path\n";
	std::cout << "----------------------\n";

	for (int v = 1; v <= size; v++) {
		std::cout << std::setw(4) << v << " :" << std::setw(8)
					<< ((dist[v] == std::numeric_limits<int>::max()) ? -1 : dist[v]) << std::setw(8)
					<< path[v] << "\n";
	}

	std::cout << "----------------------\n";
}

// print shortest path from s to t
void Digraph::printPath(int t) const {
	if (t < 1 || t > size) {
		std::cout << "\nERROR: expected target t in range 1.." << size << " !\n";
		return;
	}

	// *** TODO *** Done
	preorderPrint(path[t]);

	std::cout << " " << t << " (" << dist[t] << ")";

	// // ******* Iterative example *******
	// Starting from the back of the path
	// std::string s = std::to_string(t) + " (" + std::to_string(dist[t]) + ")";

	// while(dist[t] != 0) {
	// 	t = path[t]; // assigning t to the previous node in path
	// 	s = std::to_string(t) + " -> " + s; // add previous node first in string
	// }
	
	// std::cout << " " + s; // formatting
}

// Recursively visits every node in the path, until it reaches the source,
// then prints the nodes on the way back to the target 
void Digraph::preorderPrint(int t) const {
	//If t is not the source
	if(dist[t] != 0) {
		//Visit the previous node in the path
		preorderPrint(path[t]);
	}
	
	std::cout << " " << t << " ->";
}

