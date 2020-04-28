#include <iostream>
#include <iomanip> //setw
#include <utility> //std::move and std::pair

#pragma once

#include "dsexceptions.h"

using namespace std;

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinarySearchTree
{
private:
	struct Node; // nested class defined in node.h

public:
	BinarySearchTree() : root{nullptr} {}

	/**
	 * Copy constructor
	 */
	BinarySearchTree(const BinarySearchTree &rhs) : root{nullptr} {
		root = clone(rhs.root);
	}

	/**
	 * Destructor for the tree
	 */
	~BinarySearchTree() {
		makeEmpty();
	}

	/**
	 * Copy assignment: copy and swap idiom
	 */
	BinarySearchTree &operator=(BinarySearchTree _copy) {
		std::swap(root, _copy.root);
		return *this;
	}

	/**
	 * Find the smallest item in the tree.
	 * Throw UnderflowException if empty.
	 */
	const Comparable &findMin() const {
		if (isEmpty()) {
			throw UnderflowException{};
		}

		return findMin(root)->element;
	}

	/**
	 * Find the largest item in the tree.
	 * Throw UnderflowException if empty.
	 */
	const Comparable &findMax() const {
		if (isEmpty()) {
			throw UnderflowException{};
		}

		return findMax(root)->element;
	}

	/**
	 * Returns true if x is found in the tree.
	 */
	bool contains(const Comparable &x) const {
		return (contains(x, root) != nullptr);
	}

	/**
	 * Test if the tree is logically empty.
	 * Return true if empty, false otherwise.
	 */
	bool isEmpty() const {
		return root == nullptr;
	}

	/**
	 * Print the tree contents in sorted order.
	 */
	void printTree(ostream &out = cout) const {
		if (isEmpty()) {
			out << "Empty tree";
		}
		else {
			//inorder(root, out);
			preorder(root, out);
		}
	}

	/**
	 * Make the tree logically empty.
	 */
	void makeEmpty() {
		root = makeEmpty(root);
	}

	/**
	 * Insert x into the tree; duplicates are ignored.
	 */
	void insert(const Comparable &x) {
		root = insert(x, root);
	}

	/**
	 * Remove x from the tree. Nothing is done if x is not found.
	 */
	void remove(const Comparable &x) {
		root = remove(x, root);
	}

	/** Return total number of existing nodes
	 *
	 * Used for debug purposes
	 */
	static int get_count_nodes() {
		return Node::count_nodes;
	}

	/** 
	 * Return a pair of values belonging to the tree corresponding to the closest predecessor and successor of a given value x.
	 */
	std::pair<Comparable, Comparable> find_pred_succ(const Comparable& x) const {
		Comparable a = root->element;
		Comparable b = root->element;
		Node *temp = root;
		
		while(temp != nullptr) {
			if(x < temp->element) {
				b = temp->element;
				temp = temp->left;
			}
			else if ( x > temp->element) {
				a = temp->element;
				temp = temp->right;
			}
			else {
				Node* _max = findMax(temp->left);
				Node* _min = findMin(temp->right);
				
				if(temp->parent) { // If not root
					// On the left side of parent
					if(temp->parent->element > temp->element) {
						a = (_max) ? _max->element : (temp->element < a) ? temp->element : a;
						b = (_min) ? _min->element : b;
					}
					// On the right side of parent
					else {
						a = (_max) ? _max->element : a;
						b = (_min) ? _min->element : (temp->element > b) ? temp->element : b;
					}
				}
				break;
			}
		}

		//a största värdet mindre än x
		//b minsta värdet större än x
		return std::pair<Comparable, Comparable> {a, b};
	}

	/** 
	 * Returns the value of the parent of the node storing value x, if it exists
	 */
	Comparable get_parent(const Comparable& x) const {
		Node* temp  = contains(x, root);
		return (temp != nullptr && temp->parent != nullptr) ? temp->parent->element : Comparable{};
	}

private:
	Node *root;

	/**
	 * Private member function to insert into a subtree.
	 * x is the item to insert.
	 * t is the node that roots the subtree.
	 * p is the parent node (defaults to nullptr)
	 * Return a pointer to the node storing x.
	 */
	Node *insert(const Comparable &x, Node *t, Node* p = nullptr) {
		if (t == nullptr) {
			t = new Node{x, nullptr, nullptr, p};
		}
		else if (x < t->element) {
			t->left = insert(x, t->left, t);
		}
		else if (t->element < x) {
			t->right = insert(x, t->right, t);
		}
		else {
			; // Duplicate; do nothing
		}

		return t;
	}

	/**
	 * Private member function to remove from a subtree.
	 * x is the item to remove.
	 * t is the node that roots the subtree.
	 * p is the parent node (defaults to nullptr) //TODO Not needed
	 * Return a pointer to the new root of the subtree that had root x
	 */
	Node *remove(const Comparable &x, Node *t) {
		if (t == nullptr) {
			return t; // Item not found
		}
		
		if (x < t->element) {
			t->left = remove(x, t->left);
		}
		else if (t->element < x) {
			t->right = remove(x, t->right);
		}
		else if (t->left != nullptr && t->right != nullptr) { // Two children
			t->element = findMin(t->right)->element;
			t->right = remove(t->element, t->right);
		}
		else {
			Node *oldNode = t;
			t = (t->left != nullptr) ? t->left : t->right;
			t != nullptr ? t->parent = oldNode->parent : nullptr;
			delete oldNode;
		}

		return t;
	}

	/**
	 * Private member function to find the smallest item in a subtree t.
	 * Return node containing the smallest item.
	 */
	Node *findMin(Node *t) const {
		if (t == nullptr) {
			return nullptr;
		}
		if (t->left == nullptr) {
			return t;
		}

		// Tail recursion can be easily replaced by a loop
		return findMin(t->left); // recursive call
	}

	/**
	 * Private member function to find the largest item in a subtree t.
	 * Return node containing the largest item.
	 */
	Node *findMax(Node *t) const {
		if (t != nullptr) {
			while (t->right != nullptr) {
				t = t->right;
			}
		}

		return t;
	}

	/**
	 * Private member function to test if an item is in a subtree.
	 * x is item to search for.
	 * t is the node that roots the subtree.
	 * Return a pointer to the node storing x, if x is found
	 * Otherwise, return nullptr
	 */
	Node *contains(const Comparable &x, Node *t) const {
		if (t == nullptr) {
			return t;
		}
		else if (x < t->element) {
			return contains(x, t->left);
		}
		else if (t->element < x) {
			return contains(x, t->right);
		}
		else {
			return t; // Match
		}
	}
	/****** NONRECURSIVE VERSION*************************
	Node *contains(const Comparable &x, Node *t) const {
		while (t != nullptr) {
			if (x < t->element) {
				t = t->left;
			} else if (t->element < x) {
				t = t->right;
			} else {
				return t;  // Match
			}
		}

		return t;  // No match
	}
	*****************************************************/

	/**
	 * Private member function to make subtree empty.
	 */
	Node *makeEmpty(Node *t) {
		if (t != nullptr) {
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}

		return nullptr;
	}

	/**
	 * Private member function to print a subtree rooted at t in sorted order.
	 * In-order traversal is used
	 */
	void inorder(Node *t, ostream &out) const {
		if (t != nullptr) {
			inorder(t->left, out);
			out << t->element << '\n';
			inorder(t->right, out);
		}
	}

	/**
	 * Private member function to display tree in pre-order.
	 */
	void preorder(Node *t, ostream &out, int counter = 2) const {
		if (t != nullptr) {
			out << setw(counter);
			out << t->element << '\n';
			
			preorder(t->left, out, counter + 2);
			preorder(t->right, out, counter + 2);
			counter -= 2;
		}
		else {
			counter -=2;
		}
	}

	/**
	 * Private member function to clone subtree.
	 */
	Node *clone(Node *t, Node* p = nullptr) const {
		if (t == nullptr) {
			return nullptr;
		}

		Node* temp = new Node{t->element};
		
		temp->left = clone(t->left, temp);
		temp->right = clone(t->right, temp);
		temp->parent = p;

		return temp;
	}

		// if (t->parent == nullptr) {
		// 	Node* _root = new Node{t->element};
		// 	_root->left = clone(t->left, _root);
		// 	_root->right = clone(t->right, _root);
		// 	return _root;

		// }
		// else {
		// 	Node* temp = new Node{t->element};
		// 	temp->parent = p;
		// 	temp->left = clone(t->left, temp);
		// 	temp->right = clone(t->right, temp);

		// 	return temp;
		// }
};

//Include the definition of class Node
#include "node.h"