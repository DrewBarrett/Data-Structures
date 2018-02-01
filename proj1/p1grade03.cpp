// File: p1grade03.cpp
//
// Small accuracy test of CBofCB class
//

#include <iostream>
#include <cstdlib>
#include <list>
using namespace std ;

#include "CBofCB.h"

static int errors=0 ;

//      cerr << "#887134##X   ==  -1: InnerCB size is incorrect.\n" ;

void checkOne(list<int> &L, CBofCB& B) {
   if ( L.size() != B.size() ) {
      cerr << "#887134#   *** ERROR *** sizes of L & B do not match\n" ;
      cerr << "#887134#       L.size() = " << L.size() << endl ;
      cerr << "#887134#       B.size() = " << B.size() << endl ;
      errors++ ;
   }

   int dataL = L.front() ;
   L.pop_front() ;
   int dataB = B.dequeue() ;
   if ( dataL != dataB ) {
      cerr << "#887134#       *** ERROR *** data in L & B do not match\n" ;
      cerr << "#887134#           dataL = " << dataL << endl ;
      cerr << "#887134#           dataB = " << dataB << endl ;
      errors++ ;
   }

}


bool CBofCB::inspect (InnerCB** &buf, int &cap, int &size, int &start, int &end) {
   buf = m_buffers ;
   cap = m_obCapacity ;
   size = m_obSize ;
   start = m_oldest ;
   end = m_newest ;

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

   int data=0 ;
   CBofCB B ;
   list<int> L ;

   cout << "\n-------------------\n" ;
   cout << "Add 37 items\n" ;

   for (int i=1 ; i <= 87 ; i++) {
      data++ ;
      L.push_back(data) ;
      B.enqueue(data) ;
   }


   cout << "\n-------------------\n" ;
   cout << "Remove 23 items\n" ;
   for (int i=1 ; i <= 23 ; i++) {
      checkOne(L,B) ;
   }


   cout << "\n-------------------\n" ;
   cout << "Add 53 items\n" ;
   for (int i=1 ; i <= 53 ; i++) {
      data++ ;
      L.push_back(data) ;
      B.enqueue(data) ;
   }


   // 
   //
   cout << "\n-------------------\n" ;
   cout << "Should have approximately 7, 40, 70 items\n" ;
   cout << "in InnerCBs 1, 2 and 3\n" ;
   reportSizes(B) ;
   // B.dump() ;
   cout << "\n-------------------\n" ;


   cout << "\n-------------------\n" ;
   cout << "Remove 117items\n" ;
   for (int i=1 ; i <= 117 ; i++) {
      checkOne(L,B) ;
   }


   if (B.isEmpty()) {
      cerr << "#887134##X   ==  -0: CBofCB is empty, it should be.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: CBofCB is not empty, it should be.\n" ;
   }

   if (errors == 0) {
      cerr << "#887134##X   ==  -0: no FIFO errors reported.\n" ;
   } else if (errors <= 5) {
      cerr << "#887134##X   ==  -1: " << errors << " FIFO errors reported.\n" ;
   } else if (errors <= 12) {
      cerr << "#887134##X   ==  -2: " << errors << " FIFO errors reported.\n" ;
   } else if (errors <= 30) {
      cerr << "#887134##X   ==  -3: " << errors << " FIFO errors reported.\n" ;
   } else if (errors <= 45) {
      cerr << "#887134##X   ==  -4: " << errors << " FIFO errors reported.\n" ;
   } else {
      cerr << "#887134##X   ==  -5: " << errors << " FIFO errors reported.\n" ;
   }

   cerr << "#887134# Reached end of main program.\n" ;
   return 0 ;
}
