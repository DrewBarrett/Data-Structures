// File: p1grade04.cpp
//
// Large accuracy test of CBofCB class
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
//      cerr << "#887134#   *** ERROR *** sizes of L & B do not match\n" ;
//      cerr << "#887134#       L.size() = " << L.size() << endl ;
//      cerr << "#887134#       B.size() = " << B.size() << endl ;
      errors++ ;
   }

   int dataL = L.front() ;
   L.pop_front() ;
   int dataB = B.dequeue() ;
   if ( dataL != dataB ) {
//      cerr << "#887134#       *** ERROR *** data in L & B do not match\n" ;
//      cerr << "#887134#           dataL = " << dataL << endl ;
//      cerr << "#887134#           dataB = " << dataB << endl ;
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
   cout << "Add 900 items\n" ;

   for (int i=1 ; i <= 900 ; i++) {
      data++ ;
      L.push_back(data) ;
      B.enqueue(data) ;
   }

   
   cout << "\n-------------------\n" ;
   cout << "Should have about 10, 20, 40, 80, 160, 320 & 270 items in InnerCBs 0 thru 6\n\n" ;
   reportSizes(B) ;


   cout << "\n-------------------\n" ;
   cout << "Remove 331 items\n" ;
   for (int i=1 ; i <= 331 ; i++) {
      L.pop_front() ;
      B.dequeue() ;
   }

   // reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Add 8000 items\n" ;
   for (int i=1 ; i <= 8000 ; i++) {
      data++ ;
      L.push_back(data) ;
      B.enqueue(data) ;
   }

   cout << "\n-------------------\n" ;
   cout << "InnerCBs should wrap around.\n" ;
   cout << "Should have about 299, 640, 1280, 2560, 3790 items in InnerCBs 5 6 0 1 2\n\n" ;
   reportSizes(B) ;


   cout << "\n-------------------\n" ;
   cout << "Remove 8569 items\n" ;
   for (int i=1 ; i <= 8569 ; i++) {
      checkOne(L,B) ;
   }


   if (B.isEmpty()) {
      cerr << "#887134##X   ==  -0: CBofCB is empty, it should be.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: CBofCB is not empty, it should be.\n" ;
   }

   if (errors == 0) {
      cerr << "#887134##X   ==  -0: no FIFO errors reported.\n" ;
   } else if (errors <= 50) {
      cerr << "#887134##X   ==  -1: " << errors << " FIFO errors reported.\n" ;
   } else if (errors <= 200) {
      cerr << "#887134##X   ==  -2: " << errors << " FIFO errors reported.\n" ;
   } else if (errors <= 2000) {
      cerr << "#887134##X   ==  -3: " << errors << " FIFO errors reported.\n" ;
   } else if (errors <= 4000) {
      cerr << "#887134##X   ==  -4: " << errors << " FIFO errors reported.\n" ;
   } else {
      cerr << "#887134##X   ==  -5: " << errors << " FIFO errors reported.\n" ;
   }

   cerr << "#887134# Reached end of main program.\n" ;
   return 0 ;
}
