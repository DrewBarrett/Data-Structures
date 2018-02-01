// File: PerfectHT.cpp
//
// UMBC CMSC 341 Fall 2017 Project 5
//
// Implementation of HashFunction, SecondaryHT & PerfectHT classes.
//
// Version: 2017-11-30
//

#include <stdexcept>
#include <iostream>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <vector>

using namespace std ;

#include "PerfectHT.h"
#include "primes.h"


// =====================================================================
// HashFunction class implementation 
// =====================================================================


// By default do not print info to screen
//
bool HashFunction::m_debug = false ;


// Constructor.
// The constructor picks the smallest prime # greater than
// or equal to n for the table size. 
// Default value of n defined in header.
//  
// Initializes other hash function parameters randomly.
//
HashFunction::HashFunction(int n /* =100*/ ) {

   // note: maxPrime defined in prime.h
   //
   if (n > maxPrime) throw out_of_range("HashTable size too big.\n") ;

   m_tsize = findPrime(n) ;
   m_factor = ( rand() % (m_tsize - 1) ) + 1 ;
   m_shift =  rand() % m_tsize ;
   m_multiplier = 33 ;  // magic number from textbook
   m_reboots = 0 ;

}


// Function that maps string to unsigned int.
// Return value can be much larger than table size.
// Uses m_multiplier data member.
// Return value must be unsgined for overflow to work correctly.
//
unsigned int HashFunction::hashCode(const char *str) const {

   unsigned int val = 0 ; 

   int i = 0 ;
   while (str[i] != '\0') {
      val = val * m_multiplier + str[i] ;
      i++ ;
   }
   return val ; 
}


// Getter for table size
//
int HashFunction::tableSize() const {
   return m_tsize ;
}


// Overloaded () operator that makes this a "functor" class.
// Returns the slot in the hash table for str.
// Uses the MAD method: h(x) = (ax + b) % m where 
// the parameters are:
//   a = m_factor
//   b = m_shift
//   m = m_tsize
//
unsigned int HashFunction::operator() (const char *str) const {
   return ( m_factor * hashCode(str) + m_shift ) % m_tsize ;
}


// Pick new parameters for MAD method and the hashCode function.
// Note: can change table size.
//
void  HashFunction::reboot() {
    m_reboots++;
    // Implement the reboot function
    // pick new values for m_factor and m_shift each reboot
    //   make sure you get *different* values
    // change m_multiplier to next odd number  every 3rd reboot
    if (m_reboots % 3 == 0) {
        m_multiplier += 2;
    }
    // increase table size to next prime # every 5th reboot
    if (m_reboots % 5 == 0) {
        m_tsize = findPrime(m_tsize + 1);
    }

    int a, b;
    
    do {
        a = ( rand() % (m_tsize - 1) ) + 1 ;
        b =  rand() % m_tsize ;
    } while (a == m_factor && b == m_shift);
    m_factor = a;
    m_shift = b;
    
    /*
    do {
        a = ( rand() % (m_tsize - 1) ) + 1 ;
    } while (a == m_factor );
    m_factor = a;
   
    do {
        b =  rand() % m_tsize ;
    } while ( b == m_shift);
    m_shift = b;
    */
    if (m_debug) {
        cout << "---- reboot #" << m_reboots << ":  "
            << "m_tsize = " << m_tsize
            << " m_multiplier = " << m_multiplier
            << " m_factor = " << m_factor
            << " m_shift = " << m_shift << endl;
    }

}


// Set random seed for the random number generator.
// Call once at the start of the main program.
// Uses srand() and rand() from cstdlib which is
// shared with other code. For compatibility with
// C++98, it does not have a private random number 
// generator (e.g., mt19337).
//
void HashFunction::setSeed(unsigned int seed) {
   srand(seed) ;
}


// Find the smallest prime number greater than or equal to m.  

int HashFunction::findPrime(int m) {
    if (m > maxPrime) {
        throw out_of_range("Table size greater than max prime!");
    }
    // Prime numbers are in global array primes[]
    // included from "primes.h"
    int first = 0;
    int last = numPrimes - 1;
    int middle;
    // Use binary search!!
    while (first <= last) {
        middle = (first + last) / 2;
        if (m_debug) {
            //cout << "middle = " << middle << endl;
            //cout << "index = " << index << endl;
            //cout << "remianing = " << remaining << endl;
        }
        
        if (m < primes[middle]) {
            // if this prime is greater than us
            // and we are greater than the preceding prime
            // then we have found the smallest greater prime.
            if (m > primes[middle - 1]) {
                return primes[middle];
            }
            last = middle - 1;
        } 
        else if (m > primes[middle]) {
            first = middle + 1;
        }
        else {
            // we are equal
            return primes[middle];
        }
    }
}



// =====================================================================
// SecondaryHT class implementation 
// =====================================================================


// By default do not print debugging info 
bool SecondaryHT::m_debug = false ;


// Create a secondary hash table using the char * strings
// stored in "words". Makes copy of each item in words.
//
SecondaryHT::SecondaryHT(vector<const char *> words) : hash(words.size() * words.size()) {
    int n = words.size();
    m_size = n;
    
    n = n * n;
    //
    // Keep trying until a table with no collisions is found
    // 
    vector<int> index;
    bool collision;
    //hash = HashFunction(n);
    m_attempts = 0;
    do {
        collision = false;
        if (m_attempts > maxAttempts) {
            throw very_unlucky("Too many attempts at secondary table!");
        }
        m_attempts++;
        if (m_attempts > 1) {
            // only reboot if we havent tried the defaults
            hash.reboot();
        }
        // keep track of every hash and look for collisions
        for (vector<const char *>::iterator it = words.begin(); it != words.end(); ++it) {
            int h = hash(*it);
            for (vector<int>::iterator jt = index.begin();jt != index.end(); ++jt) {
                if (h == *jt) {
                    collision = true;
                    break;
                }  
            }
            if (collision) {
                break;
            }
            index.push_back(h);
        }
        index.clear(); // reset index tracking
    } while (collision == true);
    // wrap debugging statements in "if (m_debug) { ... }"
    //
    // remember that calling HashFunction::reboot() can change 
    // the table size!
    T2 = new char*[hash.tableSize()];
    for (int i = 0; i < hash.tableSize(); i++) {
        T2[i] = NULL;
    }
    //
    // use strdup to copy char * strings
    //

    for (vector<const char *>::iterator it = words.begin(); it != words.end(); ++it) {
        int h = hash(*it);
        T2[h] = strdup(*it);
    }
    if (m_debug) {
        cout << "==== Called SecondaryHT ====\n"
            << "SecondaryHT constructor, # attempts = "
            << m_attempts << endl;
    }
}


// Copy constructor. Remember to strdup.
//
SecondaryHT::SecondaryHT(const SecondaryHT& other) :
m_size(other.m_size), hash(other.hash), m_attempts(other.m_attempts) {
    T2 = new char*[hash.tableSize()];
    for (int i = 0; i < hash.tableSize(); i++) {
        if (other.T2[i] == NULL) {
            T2[i] = NULL;
        }
        else {
            T2[i] = strdup(other.T2[i]);
        }
    }
}


// Destructor. Use free() to free strings.
//
SecondaryHT::~SecondaryHT() {
    for (int i = 0; i < hash.tableSize(); i++) {
        if (T2[i] == NULL) {
            T2[i] = NULL;
        }
        else {
            free(T2[i]);
        }
    }
    delete [] T2;
}


// Assignment operator. Remember to use strdup() and free()
//
const SecondaryHT& SecondaryHT::operator=(const SecondaryHT& rhs) {
    if (this == &rhs)
        return *this;
    for (int i = 0; i < hash.tableSize(); i++) {
        if (T2[i] == NULL) {
            T2[i] = NULL;
        }
        else {
            free(T2[i]);
        }
    }
    delete [] T2;
    m_size = rhs.m_size;
    hash = rhs.hash;
    m_attempts = rhs.m_attempts;
    T2 = new char*[hash.tableSize()];
    for (int i = 0; i < hash.tableSize(); i++) {
        if (rhs.T2[i] == NULL) {
            T2[i] = NULL;
        }
        else {
            T2[i] = strdup(rhs.T2[i]);
        }
    }
    return *this;
}


// returns whether given word is in this hash table.
//
bool SecondaryHT::isMember (const char *word) const {

   int slot = hash(word) ;

   assert ( 0 <= slot && slot < hash.tableSize() ) ;

   if (T2[slot] == NULL) return false ;

   if ( strcmp(word, T2[slot]) != 0 ) return false ;

   return true ; 
}


// getter. Actually, the hash function has the size.
//
int SecondaryHT::tableSize() const {
   return hash.tableSize() ;
}


// Pretty print for debugging
//
void SecondaryHT::dump() const {
   cout << "=== Secondary Hash Table Dump: ===\n" ;
   cout << "Table size = " << hash.tableSize() << endl ;
   cout << "# of items = " << m_size << endl ;
   cout << "# of attempts = " << m_attempts << endl ;
 
   for (int i=0 ; i < hash.tableSize() ; i++) {
      cout << "T2[" << i << "] = " ;
      if (T2[i] != NULL) cout << T2[i] ;
      cout << endl ;
   }

   cout << "==================================\n" ;
}



// =====================================================================
// PerfectHT class implementation 
// =====================================================================


// by default, do not print debugging info
//
bool PerfectHT::m_debug = false ;


// Create a Perfect Hashing table using the first n strings 
// from the words array.
//
PerfectHT::PerfectHT (const char *words[], int n) :
hash(HashFunction(n)) {
    PHT1 = new char*[hash.tableSize()];
    PHT2 = new SecondaryHT*[hash.tableSize()];
    // Implement constructor for PerfectHT here.
    //
    // You will need an array of vectors of char * strings.
    // Something like;
    //

    vector<const char *> *hold = new vector<const char *>[hash.tableSize()] ;
    for (int i = 0; i < hash.tableSize(); i++) {
        PHT1[i] = NULL;
        PHT2[i] = NULL;
        hold[hash(words[i])].push_back(words[i]);
    }
    //
    // Each hold[i] is a vector that holds the strings
    // that hashed to slot i. Then hold[i] can be passed
    // to the SecondaryHT constructor.
    //
    for (int i = 0; i < hash.tableSize(); i++) {
        if (hold[i].size() > 1) {
            PHT2[i] = new SecondaryHT(hold[i]);
        }
        else if (hold[i].size() == 1) {
            PHT1[i] = strdup(hold[i].at(0));
        }
        // otherwise both are null
    }
    
    delete [] hold;
}


// Copy constructor. Use strdup or SecondaryHT copy constructor.
//
PerfectHT::PerfectHT(const PerfectHT& other) : 
hash(other.hash){
    PHT1 = new char*[hash.tableSize()];
    PHT2 = new SecondaryHT*[hash.tableSize()];
    for (int i = 0; i < hash.tableSize(); i++) {
        if (other.PHT1[i] != NULL) {
            PHT1[i] = strdup(other.PHT1[i]);
        } else {
            PHT1[i] = NULL;
        }
        if (other.PHT2[i] != NULL) {
            PHT2[i] = new SecondaryHT(*other.PHT2[i]);
        } else {
            PHT2[i] = NULL;
        }
    }
}


// Destructor. Remember to use free() for strdup-ed strings.
//
PerfectHT::~PerfectHT() {
    for (int i = 0; i < hash.tableSize(); i++) {
        if (PHT1[i] != NULL) {
            free(PHT1[i]);
        }
        if (PHT2[i] != NULL) {
            delete PHT2[i];
        }
    }
    delete [] PHT2;
    delete [] PHT1;
}


// Assignment operator. Use strdup() and free() for strings.
//
const PerfectHT& PerfectHT::operator=(const PerfectHT& rhs) {
    if (this == &rhs)
        return *this;
    for (int i = 0; i < hash.tableSize(); i++) {
        if (PHT1[i] != NULL) {
            free(PHT1[i]);
        }
        if (PHT2[i] != NULL) {
            delete PHT2[i];
        }
    }
    delete [] PHT2;
    delete [] PHT1;
    hash = rhs.hash;
    PHT1 = new char*[hash.tableSize()];
    PHT2 = new SecondaryHT*[hash.tableSize()];
    for (int i = 0; i < hash.tableSize(); i++) {
        if (rhs.PHT1[i] != NULL) {
            PHT1[i] = strdup(rhs.PHT1[i]);
        } else {
            PHT1[i] = NULL;
        }
        if (rhs.PHT2[i] != NULL) {
            PHT2[i] = new SecondaryHT(*rhs.PHT2[i]);
        } else {
            PHT2[i] = NULL;
        }
    }
    return *this;
}


// Returns whether word is stored in this hash table.
//
bool PerfectHT::isMember(const char * str) const {
   int slot = hash(str) ; 

   if (PHT1[slot] == NULL && PHT2[slot] == NULL) return false ;

   if (PHT1[slot] != NULL) return strcmp(str,PHT1[slot]) == 0 ;

   return PHT2[slot]->isMember(str) ;
}


// Pretty print for debugging.
//
void PerfectHT::dump() const {
   int m = hash.tableSize() ;   

   cout << "------------- PerfectHT::dump()  -------------\n" ;
   cout << "table size = " << hash.tableSize() << endl ;
   cout << endl ;
   for (int j=0 ; j < m ; j++) {
      cout << "[" << j << "]:  " ;
      if (PHT1[j] != NULL) cout << PHT1[j] ;
      if (PHT2[j] != NULL) {
         cout << endl ;
         PHT2[j]->dump() ;
      }
      cout << endl ;
   }

   cout << "----------------------------------------------\n" ;
}
