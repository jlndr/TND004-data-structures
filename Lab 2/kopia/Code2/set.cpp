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
    // IMPLEMENT before HA session on week 16
}

// Conversion constructor
Set::Set(int n)
    : Set{}  // create an empty list
{
    // IMPLEMENT before HA session on week 16
}

// Constructor to create a Set from a sorted vector v
Set::Set(const std::vector<int>& v)
    : Set{}  // create an empty list
{
    // IMPLEMENT before HA session on week 16
}

// Make the set empty
void Set::make_empty() {
    // IMPLEMENT before HA session on week 16
}

Set::~Set() {
    // Member function make_empty() can be used to implement the destructor
    // IMPLEMENT before HA session on week 16
}

// Copy constructor
Set::Set(const Set& source)
    : Set{}  // create an empty list
{
    // IMPLEMENT before HA session on week 16
}

// Copy-and-swap assignment operator
Set& Set::operator=(Set source) {
    // IMPLEMENT before HA session on week 16

    return *this;
}

// Test whether a set is empty
bool Set::is_empty() const {
    return (counter == 0);
}

// Test set membership
bool Set::is_member(int val) const {
    // IMPLEMENT before HA session on week 16

    return false;  // remove this line
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
    // IMPLEMENT

    return *this;
}

// Modify *this such that it becomes the intersection of *this with Set S
Set& Set::operator*=(const Set& S) {
    // IMPLEMENT

    return *this;
}

// Modify *this such that it becomes the Set difference between Set *this and Set S
Set& Set::operator-=(const Set& S) {
    // IMPLEMENT

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


