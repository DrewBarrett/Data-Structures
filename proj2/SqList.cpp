#include "SqList.h"
#include <stdexcept>
#include <iostream>
#include <math.h>

// must run in O(1)
SqList::SqList() {
    m_size = 0;
}

// copies n times
SqList::SqList(const SqList & other) {
    m_size = other.m_size;
    // iterate over others list and use list copy constructor to insert them into ours
    for (list< list<Int341> >::const_iterator it = other.L.begin(); it != other.L.end(); ++it) {
        list<Int341> copy = list<Int341>(*it); // calls copy constructor for every element
        L.push_back(copy);
    }
}

SqList::~SqList() {
    // TODO
}

const SqList & SqList::operator=(const SqList & rhs) {
    if (this == &rhs) {
        return *this;
    }
    m_size = rhs.m_size;
    L.clear();
    for (list< list<Int341> >::const_iterator it = rhs.L.begin(); it != rhs.L.end(); ++it) {
        list<Int341> copy = list<Int341>(*it);
        L.push_back(copy);
    }
    return *this;
}

// O(sqrt(n))
void SqList::consolidate() {
    // traverse the top level list (n)
    //cout << "DEBUG: Consolidating\n";
    //dump();
    //Int341::report();
    int pos = 0;
    bool eraseLast = false;
    for (list< list<Int341> >::iterator it = L.begin(); it != L.end(); ++it) {
        pos++; // NOTE: This is position starting at one so you must -1 from it if you want index
        //cout << "DEBUG: We have a size of " << it->size() << endl;
        // If the inner list is empty we delete it
        double shortNum = sqrt(m_size) / 2;
        //double shortNum = 4 * ((m_size) * (m_size));
        double longNum = sqrt(m_size) * 2;
        //double longNum = ((m_size)* (m_size)) / 4;
        // if the left element is empty erase it after we've iterated over it 
        if (eraseLast) {
            //cout << "we are erasing the last element";
            list< list<Int341> >::iterator left = L.begin();
            advance(left, pos - 2); // pos - 2 is left element
            //cout << left->size();
            L.erase(left);
            eraseLast = false;
        }
        //cout << "size: " << it->size() << " longNum: " << longNum << endl;
        // if the current element is empty we queue it up to be erased
        if (it->empty()) {
            //cout << "we are setting ourselves up to be erased next turn\n";
            eraseLast = true;
        }
        // If the list to the right of us is short (and we are) then we merge with it
        // short is defined as having less than sqrt(n) / 2 items
        else if (it->size() < shortNum) {
            // also look at the one to the right of us
            list< list<Int341> >::iterator right = L.begin();
            advance(right, pos);
            //cout << "it: " << it->size() << " right: " << right->size() << endl;
            if (right != L.end() && right->size() < shortNum) {
                //cout << "DEBUG: Merge lists\n";
                // the right list is smaller lets splice them together
                it->splice(it->end(), *right); // O(c)
                // delete now empty right list
                L.erase(right);
                //dump();
            }
        }
        // if we werent too short we could be too big
        // big is having more than 2 sqrt(n) items
        // in this case we insert a new list and move half of the items over
        else if (it->size() > longNum) {
            //cout << "DEBUG: Splitting list\n";
            // insert at pos inserts before pos so we wont have to loop over extra list
            L.insert(it, list<Int341>());
            list<Int341>::iterator middle = it->begin();
            list< list<Int341> >::iterator newList = L.begin();
            // Point the newList iterator to the list we just inserted
            advance(newList, pos - 1);
            // point the middle iterator at the middle of current list
            advance(middle, it->size() / 2);
            newList->splice(newList->begin(), *it, it->begin(), middle); // linear time proportional to size
            pos++; // we are a position further now!
        }
    }

    // if the last element of the list is empty
    if (eraseLast) {
        L.pop_back();
    }
    // cout << "Finished Consolidating\n";
    //dump();
    //Int341::report();
}

// constant time
void SqList::addFirst(const Int341 & x) {
    add(0, x); // adding to front takes constant time
}

// constant time
void SqList::addLast(const Int341 & x) {
    add(m_size, x); // adding to end takes constant time
}

Int341 SqList::removeFirst() {
    // calling remove() will take constant time
    return remove(0);
}

// constant time
Int341 SqList::removeLast() {
    // we cannot call remove() because it will takes sqrt(n) time to get to end
    if (m_size == 0) {
        throw out_of_range("List is empty you cannot remove from it");
    }
    list<Int341> & lastList = L.back();
    Int341 & removed = lastList.back();
    lastList.pop_back();
    m_size--;
    consolidate();
    return removed;
}

// sqrt(n) time
void SqList::add(int pos, const Int341 & data) {
    
    // first we check to see if pos is valid
    // O(c)
    if (pos < 0 || pos > m_size) {
        // if pos is negative or greater than current amount of items stored
        throw out_of_range("Position is out of range to add");
    }
    // if there are no lists in the list then we had better add one
    // O(c)
    if (L.size() == 0) {
        cout << "DEBUG: Creating new list\n";
        list<Int341> newList;
        L.push_back(newList);
    }
    // if pos is m_size then we add to end
    // O(c)
    if (pos == m_size) {
        // append to last list
        L.back().push_back(data);
    }
    else {
        // We add to position
        // algorithm to add to certain position that takes sqrt(n) time
        int currPos = 0;
        for(list< list<Int341> >::iterator it = L.begin(); it != L.end(); ++it) {
            // check if this list could contain the position we want
            if (currPos + (it->size() - 1) >= pos) {
                int listPos = pos - currPos;
                list<Int341>::iterator it2 = it->begin(); 
                advance(it2, listPos);
                it->insert(it2, data);
                break;
            }
            currPos += (it->size());
        }

    }

    m_size++;

    //dump(); //debug
    // not counted in time
    consolidate();
    //dump(); //debug
}

// O(sqrt(n))
Int341 SqList::remove(int pos) {
    // first we check to see if pos is valid
    // O(c)
    if (pos < 0 || pos >= m_size) {
        // if pos is negative or greater than current amount of items stored
        throw out_of_range("Position is out of range to remove");
    }
    // algorithm to remove from certain position O(sqrt(n))
    int currPos = 0;
    Int341 * removed;
    for(list< list<Int341> >::iterator it = L.begin(); it != L.end(); ++it) {
        // check if this list could contain the position we want
        if (currPos + (it->size() - 1) >= pos) {
            int listPos = pos - currPos;
            list<Int341>::iterator it2 = it->begin(); 
            advance(it2, listPos);
            removed = &*it2;
            cout << "the iterator we are removing: " << it2->m_data << " and the current size" << m_size << " " << L.size() << " " << L.begin()->size();
            it->erase(it2); // constant time
            break;
        }
        currPos += (it->size());
    }

    m_size--;

    consolidate();
    return *removed;
}

// O(sqrt(n))
Int341 & SqList::operator[](int pos) {
    // first we check to see if pos is valid
    // O(c)
    if (pos < 0 || pos >= m_size) {
        // if pos is negative or greater than current amount of items stored
        throw out_of_range("Position is out of range to get");
    }
    // algorithm to get from certain position O(sqrt(n))
    int currPos = 0;
    Int341 * toReturn;
    for (list< list<Int341> >::iterator it = L.begin(); it != L.end(); ++it) {
        // check if this list could contain the position we want
        if (currPos + (it->size() - 1) >= pos) {
            int listPos = pos - currPos;
            list<Int341>::iterator it2 = it->begin();
            advance(it2, listPos);
            toReturn = &*it2;
            break;
        }
        currPos += (it->size());
    }
    return *toReturn;
}

int SqList::indexOf(const Int341 & data) {
    // loop over every item O(n)
    int pos = 0;
    for (list< list<Int341> >::iterator it = L.begin(); it != L.end(); ++it) {
        for (list<Int341>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
            if (it2->m_data == data.m_data) {
                return pos;
            }
            pos++;
        }
    }
    return -1;
}

int SqList::numItems() {
    return m_size;
}

void SqList::dump() {
    cout << "----- SqList dump() ----\n";
    cout << "number of items = " << m_size << endl;
    cout << "------------------------\n";
    
    // iterate over outer list
    int pos = 0;
    for (list< list<Int341> >::iterator it = L.begin(); it != L.end(); ++it) {
        cout << pos << " (" << it->size() << ") :";
        // iterate over inner list
        for (list<Int341>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
            cout << " " << it2->m_data;
        }
        cout << endl;
        pos++;
    }
    cout << "------------------------\n";
}
