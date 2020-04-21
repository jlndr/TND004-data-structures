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
Set::Set() : counter{0}, head{new Node{}}, tail{new Node{0, nullptr, head}}{
    // IMPLEMENT before HA session on week 16
    head->next = tail;
}

// Conversion constructor
Set::Set(int n)
    : Set{}  // create an empty list
{
    // IMPLEMENT before HA session on week 16
    insert(tail, n);
}

// Constructor to create a Set from a sorted vector v
Set::Set(const std::vector<int>& v)
    : Set{}  // create an empty list
{
    // IMPLEMENT before HA session on week 16
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        insert(tail, *it);
    }
}

// Make the set empty
void Set::make_empty() {
    // IMPLEMENT before HA session on week 16
    Node* p = head->next;
    
    while(p->next){
        Node* pNext = p->next;
        erase(p);
        p = pNext;
    }
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
    // IMPLEMENT before HA session on week 16
    Node* p = source.head->next;
    
    while (p->next)
    {
        insert(tail, p->value);
        p = p->next;
    }
}

// Copy-and-swap assignment operator
Set& Set::operator=(Set source) {
    // IMPLEMENT before HA session on week 16
    // swap the nodes of L with *this
    // by swapping the pointers
    std::swap(head, source.head);
    std::swap(tail, source.tail);
    std::swap(counter, source.counter);
    
    return *this;
}

// Test whether a set is empty
bool Set::is_empty() const {
    return (counter == 0);
}

// Test set membership
bool Set::is_member(int val) const {
    // IMPLEMENT before HA session on week 16
    Node* p = head->next;
    while(p->value){
        if(p->value == val) return true;
        p = p->next;
    }
    
    return false; //key not found
}

// Return number of elements in the set
size_t Set::cardinality() const {
    return counter;
}

// Return true, if the set is a subset of b, otherwise false
// a <= b iff every member of a is a member of b
bool Set::operator<=(const Set& b) const {
    // IMPLEMENT
//    Node* p1 = head->next;
//    Node* p2 = b.head->next;
//
//    while( p1->next && p2->next ) {
//        if( p1->value > p2->value){
//            p2 = p2->next;
//        }
//        else if( p1->value < p2->value ) {
//            return false;
//        }else {
//            p1 = p1->next;
//            p2 = p2->next;
//        }
//    }
//
//    if( p1->next == nullptr ) return true;
//
//    return false;
    return (*this * b).counter == counter;
}

// Return true, if the set is equal to set b
// a == b, iff a <= b and b <= a
bool Set::operator==(const Set& b) const {
    // IMPLEMENT
    return (*this <= b && b <= *this);
}

// Return true, if the set is different from set b
// a != b, iff a <= b and b <= a
bool Set::operator!=(const Set& b) const {
    // IMPLEMENT
    return !(*this <= b && b <= *this);
}

// Return true, if the set is a strict subset of S, otherwise false
// a < b, iff a <= b but not b <= a
bool Set::operator<(const Set& b) const {
    // IMPLEMENT
    return (*this <= b && !(b <= *this));  // remove this line
}

// Modify *this such that it becomes the union of *this with Set S
// Add to *this all elements in Set S (repeated elements are not allowed)
Set& Set::operator+=(const Set& S) {
    // IMPLEMENT
    Node* p1 = head->next;
    Node* p2 = S.head->next;
    
    while( p1->next && p2->next ) {
        if( p1->value == p2->value) {
            p1 = p1->next;
            p2 = p2->next;
        }
        else if(p2->value < p1->value){
            insert(p1, p2->value);
            p2 = p2->next;
        }else {
            p1 = p1->next;
        }
    }
    
    // If set S is bigger than this.
    while(p2->next){
        insert(tail, p2->value);
        p2 = p2->next;
    }
        
    return *this;
}

// Modify *this such that it becomes the intersection of *this with Set S
Set& Set::operator*=(const Set& S) {
    // IMPLEMENT
    Node* p1 = head->next;
    Node* p2 = S.head->next;
    
    while( p1->next && p2->next ) {
        if( p1->value > p2->value){
            p2 = p2->next;
        }
        else if( p1->value < p2->value ) {
            p1 = p1->next;
            erase(p1->prev);
        }else {
            p1 = p1->next;
            p2 = p2->next;
        }
    }
    
    while( p1->next ){
        p1 = p1->next;
        erase(p1->prev);
    }
        
    return *this;
}

// Modify *this such that it becomes the Set difference between Set *this and Set S
Set& Set::operator-=(const Set& S) {
    // IMPLEMENT
    Node* p1 = head->next;
    Node* p2 = S.head->next;
    
    while( p1->next && p2->next ) {
        if( p1->value > p2->value){
            p2 = p2->next;
        }
        else if( p1->value < p2->value ) {
            p1 = p1->next;
        }else {
            p1 = p1->next;
            p2 = p2->next;
            erase(p1->prev);
        }
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
void Set::insert(Node *p, int val)
{
    Node* newNode = new Node(val, p, p->prev); //steps 1 and 2
    p->prev = p->prev->next = newNode; //steps 3 and 4
    counter++;
}

// Remove the Node pointed by p
void Set::erase(Node* p) {
    p->prev->next = p->next;
    p->next->prev = p->prev;
    counter--;

    delete p;  // deallocate the memory
}
