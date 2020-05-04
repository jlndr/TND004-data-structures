#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cassert> 
#include <algorithm>  //std::sort
#include <string>
#include <cctype>

/**
 * Authors:
 * Victor Lindquist, vicli268
 * Johan Linder, johli153
 * 
 */

#include "BinarySearchTree.h"
#include "iterator.h"

template <typename Comparable>
class FrequencyPair {
public:
	
	FrequencyPair() = delete;
	
	FrequencyPair(const Comparable& c)
		: counter{ 1 }, element{ c }
	{}

	~FrequencyPair() = default;

	int getCounter() { return counter; }

	bool operator<(const FrequencyPair& rhs) const {
		return (element < rhs.element);
	}
	
	void operator++() { ++counter; }
	// FrequencyPair& opertor++(int) { ++(*this); }

	friend std::ostream& operator<<(std::ostream& os, const FrequencyPair& fp) {
		os << fp.element << ": " << fp.counter;
		return os;
	}

private:
	int counter;
	Comparable element;
};

bool compare(FrequencyPair<std::string> fp1, FrequencyPair<std::string> fp2) {
	return fp1.getCounter() > fp2.getCounter();
}

int main() {
	std::ifstream file{"./other files/text_long.txt"};

	if (!file) {
			std::cout << "Couldn't open file words.txt\n";
			return 1;
	}

	std::vector<std::string> V{std::istream_iterator<string>{file},
															std::istream_iterator<string>{}};

	file.close();

	// Create a tree
	BinarySearchTree<FrequencyPair<std::string>> BST{};
	

	for(auto word : V) {
		word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);
		
		FrequencyPair fp{word};
		auto it = BST.contains(fp);

		if(it != BST.end()) it->operator++();
		else BST.insert(fp);
	}

	std::cout << "\n======== Sorted alphabetically ========\n";
	for(auto it = BST.begin(); it != BST.end(); ++it) {
		std::cout << *it << "\n";
	}

	std::cout << "\n========= Sorted by frequency =========\n";
	//sort by frequency
	std::vector<FrequencyPair<std::string>> _copy;
	for(auto it = BST.begin(); it != BST.end(); ++it) {
		_copy.push_back(*it);
	}

	std::sort(_copy.begin(), _copy.end(), compare);

	for(auto fp : _copy) {
		std::cout << fp << "\n";
	}

	// std::vector<std::string>* test = new std::vector<std::string> {"hej", "hejdå", "då"};
	// std::vector<std::string>::iterator it1 = std::begin(*test);
	// std::cout << *it1 << std::endl;
	// delete test;
	// std::cout << *it1 << std::endl;

	return 0;
}