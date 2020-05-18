/*********************************************
 * file:	~\tnd004\lab\lab4b\graph.h        *
 * remark:implementation of undirected graphs *
 * 
 * Authors:
 * Victor Lindquist, vicli268
 * Johan Linder, johli153
 **********************************************/

#include <iostream>
#include <iomanip>
#include <cassert>  //assert
#include <limits>   //std::numeric_limits

// using namespace std;

#include "graph.h"
#include "edge.h"
#include "heap.h"
#include "dsets.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

// Create an empty graph with n vertices
Graph::Graph(int n) : size{n}, n_edges{0} {
	assert(n >= 1);

	table.resize(size + 1);
}

Graph::Graph(const std::vector<std::tuple<int, int, int>> &V, int n) : Graph{n} {
	for (auto [u, v, w] : V)  // C++17
	{
		insertEdge(u, v, w);
	}
}

// -- MEMBER FUNCTIONS

// insert undirected edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Graph::insertEdge(int u, int v, int w) {
	assert(u >= 1 && u <= size);
	assert(v >= 1 && v <= size);
	table[u].insert(v, w);
	table[v].insert(u, w);

	n_edges += 2;
}

// remove undirected edge (u, v)
void Graph::removeEdge(int u, int v) {
	assert(u >= 1 && u <= size);
	assert(v >= 1 && v <= size);
	table[u].remove(v);
	table[v].remove(u);

	n_edges -= 2;
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const {
	// *** TODO *** Done
	int* dist = new int[size + 1];
	int* path = new int[size + 1];
	bool* done = new bool[size + 1];

	const int MAX = std::numeric_limits<int>::max();
	
	for(int i = 1; i <= size; ++i) {
		dist[i] = MAX;
		path[i] = 0;
		done[i] = false;
	}
	//start vertice
	int s = 1;
	dist[s] = 0;
	done[s] = true;
	int totalWeight = 0;
	
	while(true) {
		Node* u = table[s].getFirst();

		while(u != nullptr) {
			//Find smallest weight
			if(done[u->vertex] == false && dist[u->vertex] > u->weight) {
				dist[u->vertex] = u->weight;
				path[u->vertex] = s;
			}

			u = table[s].getNext();
		}

		// Initialize min value 
		int min = MAX, minIndex;
	
		//Find min value
		for(int i = 1; i < size+1; ++i) {
			if(done[i] == false && dist[i] < min) {
				min = dist[i];
				minIndex = i;
			}
		}
		
		//if every vertex is visited, all done.
		if(min == MAX) break;

		s = minIndex;

		//Print
		std::cout << "(" << path[s] << ", " << s << ", " << dist[s] <<")\n";
		
		done[s] = true;
		totalWeight += dist[s];
	}

	std::cout << "\nTotal weight: " << totalWeight << "\n";

	delete[] dist;
	delete[] path;
	delete[] done;
}

// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const {
	// *** TODO *** Done
	Heap<Edge> H{n_edges};
	DSets D{size};

	/*----------- HEAPIFY ---------- */
	for(int i = 1; i <= size; ++i) {
		Node* current = table[i].getFirst();

		while(current) {
			//Insert unique edges
			if(i < current->vertex) {
				H.insert(Edge{i, current->vertex, current->weight});
			}
			current = table[i].getNext();
		}
	}
	/*------------------------------ */
	
	int counter = 0, total = 0;

	while(counter < size - 1){
		//Get the edge with lowest weight
		Edge e = H.deleteMin();

		int headRoot = D.find(e.head);
		int tailRoot = D.find(e.tail);
		
		if(headRoot != tailRoot) {
			std::cout << e << "\n"; //display edge
			D.join(headRoot, tailRoot); //
			++counter;
			total += e.weight;
		}
	}

	std::cout << "\nTotal weight: " << total << "\n";
}

// print graph
void Graph::printGraph() const {
	std::cout << "------------------------------------------------------------------\n";
	std::cout << "vertex  adjacency list                                            \n";
	std::cout << "------------------------------------------------------------------\n";

	for (int v = 1; v <= size; v++) {
		std::cout << std::setw(4) << v << " : ";
		table[v].print();
	}

	std::cout << "------------------------------------------------------------------\n";
}
