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
Set::Set()
	: counter{0}
{
	head = new Node(0, nullptr, nullptr);
	tail = new Node(0, nullptr, nullptr);
	head->next = tail;
	tail->prev = head;
}

// Conversion constructor
Set::Set(int n)
	: Set{}  // create an empty list
{
	Node* ptr = new Node(n, tail, head);
	head->next = ptr;
	tail->prev = ptr;
	++counter;
}

// Constructor to create a Set from a sorted vector v
Set::Set(const std::vector<int>& v)
	: Set{}  // create an empty list
{
	Node* ptr = head;
	
	for(int item : v) {
		ptr->next = new Node(item, nullptr, ptr);;
		ptr = ptr->next;
		++counter;
	}
	ptr->next = tail;
	tail->prev = ptr;
}

// Make the set empty
void Set::make_empty() {
	if(head->next == tail) return;
		
	Node* ptr = head->next;

	while(ptr != tail) {
		ptr = ptr->next;
		delete ptr->prev;
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
	Node* ptr_source = source.head->next;
	Node* ptr_this = head;

	while(ptr_source != source.tail) {
		ptr_this->next = new Node(ptr_source->value, nullptr, ptr_this);
		ptr_source = ptr_source->next;
		ptr_this = ptr_this->next;
	}

	ptr_this->next = tail;
	tail->prev = ptr_this;
	counter = source.counter;
}

// Copy-and-swap assignment operator
Set& Set::operator=(Set source) {
	std::swap(head, source.head);
	std::swap(tail, source.tail);

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

	Node* ptr = head->next;
	while(ptr != tail){
		if(ptr->value == val) return true;
		ptr = ptr->next;
	}

	return false;
}

// Return number of elements in the set
size_t Set::cardinality() const {
	return counter;
}

// Return true, if the set is a subset of b, otherwise false
// a <= b if every member of a is a member of b
bool Set::operator<=(const Set& b) const {
	Node* ptr_this = head->next;
	Node* ptr_b = b.head->next;

	while(ptr_this != tail && ptr_b != b.tail){
		if(ptr_this->value > ptr_b->value) {
			ptr_b = ptr_b->next;
			continue;
		}
		
		if(ptr_this->value != ptr_b->value) return false;
		ptr_this = ptr_this->next;
		ptr_b = ptr_b->next; 
	}
	
	return true;
	// return (*this * b).counter == counter;
}

// Return true, if the set is equal to set b
// a == b, if a <= b and b <= a
bool Set::operator==(const Set& b) const {
	if(counter != b.counter) return false;
	return (*this <= b && b <= *this);
}

// Return true, if the set is different from set b
// a == b, iff a <= b and b <= a
bool Set::operator!=(const Set& b) const {
	if(counter != b.counter) return true;
	return !(*this <= b && b <= *this);
}

// Return true, if the set is a strict subset of S, otherwise false
// a == b, iff a <= b but not b <= a
bool Set::operator<(const Set& b) const {
	// if(counter == b.counter) return false;
	return (counter != b.counter && *this <= b);
}

// Modify *this such that it becomes the union of *this with Set S
// Add to *this all elements in Set S (repeated elements are not allowed)
Set& Set::operator+=(const Set& S) {
	if(is_empty()) {
		*this = S;
		return *this;
	}

	Node* ptr_this = head->next;
	Node* ptr_s = S.head->next;

	while(ptr_s != S.tail && ptr_this != tail) {
		if(ptr_this->value == ptr_s->value) {
			ptr_this = ptr_this->next;
			ptr_s = ptr_s->next;
		}
		else if(ptr_this->value > ptr_s->value) {
			insert(ptr_this, ptr_s->value);
			ptr_s = ptr_s->next;
		}
		else {
			ptr_this = ptr_this->next;
		}
	}

	// if s is larger than *this
	while(ptr_s != S.tail) {
		insert(tail, ptr_s->value);
		ptr_s = ptr_s->next;
	}

	return *this;
}

// Modify *this such that it becomes the intersection of *this with Set S
Set& Set::operator*=(const Set& S) {
	if(is_empty() || S.is_empty()) {
		*this = Set{};
		return *this;
	}

	Node* ptr_this = head->next;
	Node* ptr_s = S.head->next;

	while(ptr_this != tail) {
		if(ptr_this->value < ptr_s->value || ptr_s == S.tail) {
			ptr_this = ptr_this->next;
			remove(ptr_this->prev);
			continue;
		}
		
		if(ptr_this->value > ptr_s->value) {
			ptr_s = ptr_s->next;
			continue;
		}
		
		ptr_this = ptr_this->next;
		ptr_s = ptr_s->next;
	}

	return *this;
}

// Modify *this such that it becomes the Set difference between Set *this and Set S
Set& Set::operator-=(const Set& S) {
	if(is_empty()) {
		return *this;
	}

	Node* ptr_this = head->next;
	Node* ptr_s = S.head->next;

	while(ptr_s != S.tail && ptr_this != tail) {
		if(ptr_this->value > ptr_s->value) {
			ptr_s = ptr_s->next;
			continue;
		}
		if(ptr_this->value < ptr_s->value) {
			ptr_this = ptr_this->next;
			continue;
		}
		
		ptr_this = ptr_this->next;
		ptr_s = ptr_s->next;
		remove(ptr_this->prev);
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

