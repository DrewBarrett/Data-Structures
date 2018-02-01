#ifndef _SQLIST_H_
#define _SQLIST_H_

#include "Int341.h"
#include <list>

using namespace std;

class SqList
{
public:
    // Default constructor 
    //  run in O(1) time
    SqList();

    // Copy Constructor
    //  must be O(n) (copy, not insert n times)
    SqList(const SqList& other);

    // Destructor
    //  no mem leaks please
    ~SqList();

    // Assignment operator
    //  must be O(n)
    const SqList& operator=(const SqList& rhs);

    // Consolidate lists
    // Traverse the top-level list.
    //  - Whenever an empty inner list is encountered, remove that inner list
    //  - Whenever two adjacent short inner lists are encountered, merge them into a single inner list
    //  - Whenever an inner list is found to have more than 2 root(n) items, split them into two lists of equal length
    // Run in O(sqrt(n)) (not counting splits)
    // Split run in time proportional to length of inner list
    // Cannot invoke constructor copy constructor or assignment operator of Int341 class
    void consolidate();

    // Insert at beginning of SqList
    //  Calls consolidate after insertion
    //  Must run in constant time
    void addFirst(const Int341& x);

    // Insert at end of sqlist
    //  Calls consolidate after insertion
    //  Must run in constant time
    void addLast(const Int341& x);

    // Delete at beginning of SqList and return
    //  Calls consolidate after deletion
    //  Must run in constant time
    //  Throws out of range on empty list
    Int341 removeFirst();

    // Delete at end of sqlist and return
    //  Calls consolidate after deletion
    //  Must run in constant time
    //  Throws out of range on empty list
    Int341 removeLast();

    // Insert an item at given position
    //  Positions start at 0
    //  If pos is the number of items then add to end of list
    //  If pos is not valid throw out of range
    //  Calls consolidate after insertion
    //  Operates in O(sqrt(n)) (not counting consolidate)
    void add(int pos, const Int341& data);

    // Removes an item at given pos and returns value
    //  positions start at 0
    //  throw out of range if pos is not valid
    //  Calls consolidate after insertion
    //  Operates in O(sqrt(n)) (not counting consolidate)
    Int341 remove(int pos);

    // Overloaded [] operator
    //  Positions start at 0
    //  if pos is not valid throw out of range
    //  O(sqrt(n))
    Int341& operator[](int pos);

    // Returns the position of the first occurance of a value in a SqList
    //  If no data then return -1
    //  pos starts at 0
    //  run in O(n) time
    int indexOf(const Int341& data);

    // Returns the number of items in a SqList
    //  Run in constant time
    int numItems();

    // debugging member function
    //  Print out the number of imtems in the SqList
    //  And for each inner list the size of that list and each item
    //      avoid calls to member functions
    void dump();

    void inspector();

private:
    int m_size;
    list< list<Int341> > L;
};


#endif // !_SQLIST_H_
