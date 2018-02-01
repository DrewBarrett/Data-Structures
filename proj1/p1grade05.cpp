// File: p1grade05.cpp
//
// Check copy constructor, destructor and assignment operator
//

#include <iostream>
using namespace std ;

#include "CBofCB.h"

int errors=0 ;

bool InnerCB::inspect (int* &buf, int &cap, int &size, int &start, int &end) {
   buf = m_buffer ;
   cap = m_capacity ;
   size = m_size ;
   start = m_start ;
   end = m_end ;

   return true ;
}


bool InnerCBEqualityTest (InnerCB& A, InnerCB& B) {
   int *bufA ;
   int capA, sizeA, startA, endA ;
   A.inspect(bufA,capA,sizeA,startA,endA) ;

   int *bufB ;
   int capB, sizeB, startB, endB ;
   B.inspect(bufB,capB,sizeB,startB,endB) ;

   if (capA != capB) {
      cout << "*** Error: capacities differ\n" ;
      cout << "    capA = " << capA << endl ;
      cout << "    capB = " << capB << endl ;
      return false ;
   }

   if (sizeA != sizeB) {
      cout << "*** Error: sizes differ\n" ;
      cout << "    sizeA = " << sizeA << endl ;
      cout << "    sizeB = " << sizeB << endl ;
      return false ;
   }

   if (startA != startB) {
      cout << "*** Error: starting indices differ\n" ;
      cout << "    startA = " << startA << endl ;
      cout << "    startB = " << startB << endl ;
      return false ;
   }

   if (endA != endB) {
      cout << "*** Error: ending indices differ\n" ;
      cout << "    endA = " << endA << endl ;
      cout << "    endB = " << endB << endl ;
      return false ;
   }

   int loc ;
   for (int i=0 ; i < sizeA ; i++) {
      loc = (startA + i) % capA ;
      if (bufA[loc] != bufB[loc]) {
	 cout << "*** Error: stored values differ\n" ;
	 cout << "    bufA[" << loc << "] = " << bufA[loc] << endl ;
	 cout << "    bufB[" << loc << "] = " << bufB[loc] << endl ;
	 return false ;
      }
   }

   return true ;
}


bool CBofCB::inspect (InnerCB** &buf, int &cap, int &size, int &start, int &end) {
   buf = m_buffers ;
   cap = m_obCapacity ;
   size = m_obSize ;
   start = m_oldest ;
   end = m_newest ;

   return true ;
}


bool CBofCBEqualityTest(CBofCB &A, CBofCB &B) {

   InnerCB **bufA ;
   int capA, sizeA, startA, endA ;
   A.inspect(bufA,capA,sizeA,startA,endA) ;

   InnerCB **bufB ;
   int capB, sizeB, startB, endB ;
   B.inspect(bufB,capB,sizeB,startB,endB) ;

   if (capA != capB) {
      cout << "*** Error: Outer buffer capacities differ\n" ;
      cout << "    capA = " << capA << endl ;
      cout << "    capB = " << capB << endl ;
      return false ;
   }

   if (sizeA != sizeB) {
      cout << "*** Error: Outer buffer sizes differ\n" ;
      cout << "    sizeA = " << sizeA << endl ;
      cout << "    sizeB = " << sizeB << endl ;
      return false ;
   }

   if (startA != startB) {
      cout << "*** Error: Outer buffer starting indices differ\n" ;
      cout << "    startA = " << startA << endl ;
      cout << "    startB = " << startB << endl ;
      return false ;
   }

   if (endA != endB) {
      cout << "*** Error: Outer buffer ending indices differ\n" ;
      cout << "    endA = " << endA << endl ;
      cout << "    endB = " << endB << endl ;
      return false ;
   }

   int loc ;
   for (int i=0 ; i < sizeA ; i++) {
      loc = (startA + i) % capA ;
      if (! InnerCBEqualityTest( *(bufA[loc]), *(bufB[loc]) )) {
	 cout << "*** Error: Inner buffers hold different values at loc =" 
	      << loc << endl ;
	 return false ;
      }
   }

   return true ;
}


void reportSizes(CBofCB &B) {
   InnerCB** buffers ;
   int cap ;
   int size ;
   int start ;
   int end ;

   B.inspect(buffers, cap, size, start, end) ;
   
   cout << "Total size = " << B.size() << endl ;

   int loc ;
   for (int i=0 ; i < size ; i++) {
      loc = (start + i) % cap ;
      cout << "m_buffers[" << loc << "]->size() = " 
           << buffers[loc]->size() << endl ;
   }
}


int main() {

   int data=29 ;
   CBofCB B ;

   // cout << "\n-------------------\n" ;
   // cout << "Add 152 items\n" ;
   for (int i=1 ; i <= 152 ; i++) {
      B.enqueue(data++) ;
   }
   // reportSizes(B) ;

   // cout << "\n-------------------\n" ;
   // cout << "Remove 117 items\n" ;
   for (int i=1 ; i <= 117 ; i++) {
      B.dequeue() ;
   }
   // reportSizes(B) ;

   // cout << "\n-------------------\n" ;
   // cout << "Add 3531 items\n" ;
   for (int i=1 ; i <= 3531 ; i++) {
      B.enqueue(data++) ;
   }
   // reportSizes(B) ;

   // cout << "\n-------------------\n" ;
   // cout << "Remove 300 items\n" ;
   for (int i=1 ; i <= 300 ; i++) {
      B.dequeue() ;
   }
   // reportSizes(B) ;

   CBofCB A(B) ;   // use copy constructor

   if (CBofCBEqualityTest(A,B)) {
      cerr << "#887134##X   ==  -0: Passed equality test 1.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: Failed equality test 1.\n" ;
   }

   CBofCB *ptr1, *ptr2 ;
   ptr1 = new CBofCB(B) ;
   ptr2 = new CBofCB(*ptr1) ;
   delete ptr1 ;    // calls destructor

   if (CBofCBEqualityTest(*ptr2,B)) {
      cerr << "#887134##X   ==  -0: Passed equality test 2.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: Failed equality test 2.\n" ;
   }

   delete ptr2 ;
  
   CBofCB X ;

   for (int i=1 ; i <= 430 ; i++) {
      X.enqueue(data++) ;
   }

   for (int i=1 ; i <= 125 ; i++) {
      X.dequeue() ;
   }

   for (int i=1 ; i <= 190 ; i++) {
      X.enqueue(data++) ;
   }

   for (int i=1 ; i <= 53 ; i++) {
      X.dequeue() ;
   }

   X = B ;   // uses assignment operator

   if (CBofCBEqualityTest(X,B)) {
      cerr << "#887134##X   ==  -0: Passed equality test 3.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: Failed equality test 3.\n" ;
   }

   CBofCB Y ;

   for (int i=1 ; i <= 121 ; i++) {
      Y.enqueue(data++) ;
   }

   for (int i=1 ; i <= 79 ; i++) {
      Y.dequeue() ;
   }

   Y = X ;   // uses assignment operator again

   if (CBofCBEqualityTest(Y,B)) {
      cerr << "#887134##X   ==  -0: Passed equality test 4.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: Failed equality test 4.\n" ;
   }

   cerr << "#887134# Reached end of main program.\n" ;
   return 0 ;
}
