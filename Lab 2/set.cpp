#include "set.h"
#include "node.h"

int Set::Node::count_nodes = 0;  // initialize total number of existing nodes to zero

/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

// Used for debug purposes
// Return number of existing nodes
int Set::get_count_nodes() {
	return Set::Node::count_nodes;
}

// Default constructor
Set::Set() : counter{0} {
	head = new Node(0, nullptr, nullptr);
	tail = new Node(0, nullptr, nullptr);
	head->next = tail;
	tail->prev = head;
}

// Conversion constructor
Set::Set(int n)
	: Set{}  // create an empty list
{
	Node* temp = new Node(n, tail, head);
	head->next = temp;
	tail->prev = temp;
	++counter;
}

// Constructor to create a Set from a sorted vector v
Set::Set(const std::vector<int>& v)
	: Set{}  // create an empty list
{
	Node* dummy = head;
	
	for(int item : v) {
		Node* temp = new Node(item, nullptr, dummy);
		dummy->next = temp;
		dummy = temp;
		++counter;
	}
	dummy->next = tail;
	tail->prev = dummy;
}

// Make the set empty
void Set::make_empty() {
	if(head->next == tail) return;
		
	Node* dummy = head->next;
		while(dummy != tail) {
			dummy = dummy->next;
			delete dummy->prev;
		}
	head->next = tail;
	tail->prev = head;
	counter = 0;
}

Set::~Set() {
	// Member function make_empty() can be used to implement the destructor
	// IMPLEMENT before HA session on week 16
	make_empty();
	delete head;
	delete tail;
}

// Copy constructor
Set::Set(const Set& source)
	: Set{}  // create an empty list
{
	Node* temp_source = source.head->next;
	Node* temp_this = head;

	while(temp_source != source.tail) {
		temp_this->next = new Node(temp_source->value, nullptr, temp_this);
		temp_source = temp_source->next;
		temp_this = temp_this->next;
	}
	temp_this->next = tail;
	tail->prev = temp_this;
	counter = source.counter;
}

// Copy-and-swap assignment operator
Set& Set::operator=(Set source) {
	Set _copy{source};

	std::swap(head, _copy.head);
	std::swap(tail, _copy.tail);

	counter = source.counter;
	
	return *this;
}

// Test whether a set is empty
bool Set::is_empty() const {
	return (counter == 0);
}

// Test set membership
bool Set::is_member(int val) const {
	if(head->next == tail ) return false;
	if(head->next->value > val || tail->prev->value < val) return false;

	Node* temp = head->next;
	while(temp != tail){
		if(temp->value == val) return true;
		temp = temp->next;
	}

	return false;
}

// Return number of elements in the set
size_t Set::cardinality() const {
	return counter;
}

// Return true, if the set is a subset of b, otherwise false
// a <= b iff every member of a is a member of b
bool Set::operator<=(const Set& b) const {
	// IMPLEMENT

	return false;  // remove this line
}

// Return true, if the set is equal to set b
// a == b, iff a <= b and b <= a
bool Set::operator==(const Set& b) const {
	// IMPLEMENT

	return false;  // remove this line
}

// Return true, if the set is different from set b
// a == b, iff a <= b and b <= a
bool Set::operator!=(const Set& b) const {
	// IMPLEMENT

	return false;  // remove this line
}

// Return true, if the set is a strict subset of S, otherwise false
// a == b, iff a <= b but not b <= a
bool Set::operator<(const Set& b) const {
	// IMPLEMENT

	return false;  // remove this line
}

// Modify *this such that it becomes the union of *this with Set S
// Add to *this all elements in Set S (repeated elements are not allowed)
Set& Set::operator+=(const Set& S) {

	if(is_empty()) {
		*this = S;
		return *this;
	}

	Node* temp_this = head->next;
	Node* temp_s = S.head->next;

	while(temp_s != S.tail) {
		if(temp_this->value > temp_s->value && temp_this->prev->value != temp_s->value) {
			// temp_this->prev->next = new Node(temp_s->value, temp_this, temp_this->prev);
			// temp_this->prev = temp_this->prev->next;
			insert(temp_this, temp_s->value);
		}
		
		if(temp_this->value < temp_s->value) {
			temp_this = temp_this->next;
			if(temp_this == tail) break;
			continue;
		}

		temp_s = temp_s->next;
	}

	return *this;
}

// Modify *this such that it becomes the intersection of *this with Set S
Set& Set::operator*=(const Set& S) {
	if(is_empty() || S.is_empty()) {
		*this = Set{};
		return *this;
	}

	Node* temp_this = head->next;
	Node* temp_s = S.head->next;

	while(temp_this != tail) {
		if(temp_this->value < temp_s->value || temp_s == S.tail) {
			temp_this = temp_this->next;
			remove(temp_this->prev);
			continue;
		}
		
		if(temp_this->value > temp_s->value) {
			temp_s = temp_s->next;
			continue;
		}
		
		temp_this = temp_this->next;
		temp_s = temp_s->next;
	}

	return *this;
}

// Modify *this such that it becomes the Set difference between Set *this and Set S
Set& Set::operator-=(const Set& S) {
	if(is_empty()) {
		return *this;
	}

	Node* temp_this = head->next;
	Node* temp_s = S.head->next;

	while(temp_s != S.tail && temp_this != tail) {
		if(temp_this->value > temp_s->value) {
			temp_s = temp_s->next;
			continue;
		}
		if(temp_this->value < temp_s->value) {
			temp_this = temp_this->next;
			continue;
		}
		
		temp_this = temp_this->next;
		temp_s = temp_s->next;
		remove(temp_this->prev);
	}

	return *this;
}

// Overloaded stream insertion operator<<
std::ostream& operator<<(std::ostream& os, const Set& b) {
	if (b.is_empty()) {
		os << "Set is empty!";
	} else {
		Set::Node* temp{b.head->next};

		os << "{ ";
		while (temp != b.tail) {
			os << temp->value << " ";
			temp = temp->next;
		}

		os << "}";
	}

	return os;
}

/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

//If you add any private member functions to class Set then write the implementation here
void Set::insert(Node *ptr, int val) {
    ptr->prev = ptr->prev->next = new Node(val, ptr, ptr->prev);
    counter++;
}

// Remove the Node pointed by p
void Set::remove(Node* ptr) {
    ptr->prev->next = ptr->next;
    ptr->next->prev = ptr->prev;
    counter--;

    delete ptr;  // deallocate the memory
}

