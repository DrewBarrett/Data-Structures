// File: p2grade04.cpp
//
// UMBC Fall 2017 CMSC 341 Project 2
//
// Testing [ ], add and remove.
//

#include <iostream> 
#include "SqList.h"
#include <list>
#include "Int341.h"

using namespace std ;

class myList : public list<int> {
public:

   // return iterator at position i
   list<int>::iterator itat(int i) {
      list<int>::iterator it = begin() ;
      advance(it,i) ;
      return it ;
   }
   
   void plus(int pos, int x) { insert(itat(pos),x) ; }

   int& operator[] (int pos) { return *itat(pos) ; }

   int minus(int pos) {
      list<int>::iterator it = itat(pos) ; 
      int answer = *it ;
      erase(it) ;
      return answer ;
   }
   
   void dump() {
      cout << "--------------------------------\n" ;
      int i=0 ;
      iterator it=begin() ;
      while( it != end() ) {
         cout << "L[" << i << "] = " << *it << endl ;
         it++ ;
         i++ ;
      }
      cout << "--------------------------------\n" ;
   }
} ;



void SqList::inspector() {

   int numErrors = 0 ;

   int n = numItems() ;

   list< list<Int341> >::iterator it ;


   // calculate total number of items in data structure
   //
   int sum = 0 ;
   for (it = L.begin() ; it != L.end() ; it++) {
      sum += it->size() ;
   }


   // report on size()
   //
   if (n == sum) {
      cerr << "#887134##X   ==  -0: numItems() reported correct size.\n" ;
   } else {
      numErrors++ ;
      cerr << "#887134##X   ==  -2: numItems() reported wrong size.\n" ;
   }


   // special case if no items
   //
   if (L.size() == 0)  return ;

   // Run down the outer list and check if there are any
   // of these errors: empty list, list too long, adjacent
   // short lists.
   //
   int prevSize, currSize ;
   bool prevIsShort = false ;
   bool currIsShort ;
   
   int index = 0 ;
   int numEmptyListError = 0 ;
   int numLongListError = 0 ;
   int numAdjShortListError = 0 ;
   
   for(it = L.begin() ; it != L.end() ; it++) {

      currSize =  it->size() ;

      if (currSize == 0) {    // empty?
         currIsShort = true ;
         numEmptyListError++ ;
         numErrors++ ;

      } else if ( (currSize-1)*(currSize-1) > 4*n ) {   // too long?
         numLongListError++ ;
         numErrors++ ;

      } else if ( 4*(currSize+1)*(currSize+1) <= n) {  // too short?
         currIsShort = true ;
	 if (prevIsShort) {  // adjacent shorts?
            numAdjShortListError++ ;
            numErrors++ ;
	 }
      } else {
         currIsShort = false ;
      }
      prevSize = currSize ;
      prevIsShort = currIsShort ;
      index++ ;
   }

   if (numErrors == 0) {
      cerr << "#887134##X   ==  -0: inspector() did not find any errors.\n" ;
   } else {
      cerr << "#887134##X   inspector() found:\n" ;
      cerr << "#887134##X      " << numEmptyListError << " empty inner lists.\n" ;
      cerr << "#887134##X      " << numLongListError << " long inner lists.\n" ;
      cerr << "#887134##X      " << numAdjShortListError << "adjacent short lists\n" ;
       
      if (numErrors < 3) {
          cerr << "#887134##X   ==  -1: deduction for " << numErrors << " errors found by inspector().\n" ;
      } else if (numErrors < 6) {
          cerr << "#887134##X   ==  -2: deduction for " << numErrors << " errors found by inspector().\n" ;
      } else {
          cerr << "#887134##X   ==  -3: deduction for " << numErrors << " errors found by inspector().\n" ;
      }
   }
}


// destructive testing of S == L
//
void destructiveTest (SqList& S, myList& L) {

   int n = L.size() ;

   if (S.numItems() != n) {
      cerr << "#887134##X   ==  -1: SqList and myList sizes differ during destructive testing.\n" ;
   } else {
      cerr << "#887134##X   ==  -0: SqList and myList report same size during destructive testing.\n" ;
   }


   int num1, num2 ;
   int numErrors = 0 ;

   for (int i=0 ; i < n ; i++) {
      num1 = S.removeFirst().m_data ;
      num2 = L.front() ;
      // cout << "num1 = " << num1 ;
      // cout << "     num2 = " << num2 << endl ;
      L.pop_front() ;
      if (num1 != num2) numErrors++ ;
   }

   if (numErrors == 0) {
      cerr << "#887134##X   ==  -0: SqList and myList hold same values during destructive testing.\n" ;
   } else {
      if (numErrors < 5) {
         cerr << "#887134##X   ==  -1: SqList and myList hold different values in " << numErrors 
              << " places during destructive Testing \n" ;
      } else if (numErrors < 10) {
         cerr << "#887134##X   ==  -2: SqList and myList hold different values in " << numErrors 
              << " places during destructive Testing \n" ;
      } else {
         cerr << "#887134##X   ==  -3: SqList and myList hold different values in " << numErrors 
              << " places during destructive Testing \n" ;
      }
   }
   
}



int main() {
   SqList S ;
   myList L ;
   int value = 1 ;
   int result ;

   Int341::m_debug = false ;

   for (int i=1 ; i <= 217 ; i++) {
     S.addLast(Int341(value)) ;
     L.push_back(value) ;
     value++ ;
   }

   for (int i=1 ; i <= 94 ; i++) {
     result = S.removeFirst().m_data ;
     L.pop_front() ;
   }

   for (int i=1 ; i <= 391 ; i++) {
     S.addFirst(Int341(value)) ;
     L.push_front(value) ;
     value++ ;
   }

   for (int i=1 ; i <= 318 ; i++) {
     result = S.removeLast().m_data ;
     L.pop_back() ;
   }

   // L.dump() ;
   
   // Spot check a few values using [] operator
   int bracketErrors=0 ;
   for (int i=57 ; i < 77 ; i++) {
      if (S[i].m_data != L[i]) bracketErrors++ ;
   }

   if (bracketErrors == 0) {
      cerr << "#887134##X   ==  -0: passed [] operator read test.\n" ;
   } else {
      cerr << "#887134##X   ==  -2: failed [] operator read test.\n" ;
   }

   // change a few values using [] operator
   for (int i=31 ; i < 131 ; i+=7) {
      S[i] = 10000 + i ;
      L[i] = 10000 + i ;
   }

   // add a few items using add() 
   for (int i=50 ; i < 200 ; i+=4) {
      S.add(i,20000 + i);
      L.plus(i,20000 + i);
   }

   // remove a few items using add() 
   for (int i=23 ; i < 150 ; i+=3) {
      S.remove(i);
      L.minus(i);
   }


   S.inspector() ;
   
   destructiveTest(S,L) ;

   cerr << "#887134# Reached end of main program.\n" ;
   return 0 ;
}
