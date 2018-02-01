// File: p1grade02.cpp
//
// Check copy constructor, destructor and assignment operator
//

#include <iostream>
using namespace std ;

#include "InnerCB.h"

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


int main() {

   int result ;
   InnerCB B(13) ;

   B.enqueue(11) ;
   B.enqueue(12) ;
   B.enqueue(13) ;
   B.enqueue(14) ;
   B.enqueue(15) ;
   B.enqueue(16) ;
   B.enqueue(17) ;
   B.enqueue(18) ;

   result = B.dequeue() ;
   result = B.dequeue() ;
   result = B.dequeue() ;
   result = B.dequeue() ;
   result = B.dequeue() ;

   B.enqueue(21) ;
   B.enqueue(22) ;
   B.enqueue(23) ;
   B.enqueue(24) ;
   B.enqueue(25) ;
   B.enqueue(26) ;
   B.enqueue(27) ;
   B.enqueue(28) ;

   InnerCB A(B) ;   // use copy constructor

   if (InnerCBEqualityTest(A,B)) {
      cerr << "#887134##X   ==  -0: Passed equality test 1.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: Failed equality test 1.\n" ;
   }

   InnerCB *ptr1, *ptr2 ;
   ptr1 = new InnerCB(B) ;
   ptr2 = new InnerCB(*ptr1) ;
   delete ptr1 ;    // calls destructor

   if (InnerCBEqualityTest(*ptr2,B)) {
      cerr << "#887134##X   ==  -0: Passed equality test 2.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: Failed equality test 2.\n" ;
   }

   delete ptr2 ;
  
   InnerCB X(13) ;

   X.enqueue(44) ;
   X.enqueue(45) ;
   X.enqueue(46) ;
   X.enqueue(47) ;
   X.enqueue(48) ;
   X.enqueue(49) ;
   X.enqueue(50) ;
   X.enqueue(51) ;
   X.enqueue(52) ;
   X.enqueue(53) ;
   X.enqueue(54) ;
   X.enqueue(55) ;
   X.dequeue() ;
   X.dequeue() ;
   X.dequeue() ;
   X.dequeue() ;
   X.dequeue() ;
   X.dequeue() ;
   X.dequeue() ;
   X.enqueue(56) ;
   X.enqueue(57) ;
   X.enqueue(58) ;
   X.enqueue(59) ;
   X.enqueue(60) ;

   X = B ;   // uses assignment operator

   if (InnerCBEqualityTest(X,B)) {
      cerr << "#887134##X   ==  -0: Passed equality test 3.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: Failed equality test 3.\n" ;
   }

   InnerCB Y(5) ;

   Y.enqueue(81) ;
   Y.enqueue(82) ;
   Y.enqueue(83) ;
   Y.dequeue() ;
   Y.dequeue() ;
   Y.enqueue(84) ;
   Y.enqueue(85) ;
   Y.dequeue() ;
   Y.dequeue() ;
   Y.enqueue(86) ;
   Y.enqueue(87) ;

   Y = X ;   // uses assignment operator again

   if (InnerCBEqualityTest(Y,B)) {
      cerr << "#887134##X   ==  -0: Passed equality test 4.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: Failed equality test 4.\n" ;
   }

   cerr << "#887134# Reached end of main program.\n" ;
   return 0 ;
}
