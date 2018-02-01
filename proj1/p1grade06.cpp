// File: p1grade06.cpp
//
// Timing test #1
//

#include <iostream>
using namespace std ;

#include "CBofCB.h"

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

   int data=1 ;
   CBofCB B ;

   for (int i=1 ; i <= 1270 ; i++) {
      B.enqueue(data++) ;
   }
   //reportSizes(B) ;

   for (int i=1 ; i <= 630 ; i++) {
      B.dequeue() ;
   }
   //reportSizes(B) ;

   for (int i=1 ; i <= 80640 ; i++) {
      B.enqueue(data++) ;
   }
   //reportSizes(B) ;


   if (B.isFull()) {
      cerr << "#887134##X   ==  -0: CBofCB::isFull() correctly reports B is full.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: CBofCB::isFull() fails to report B is full.\n" ;
   }


   for (int i=1 ; i <= 40320 ; i++) {
      B.dequeue() ;
   }
   //reportSizes(B) ;

   for (int i=1 ; i <= 5160960 ; i++) {
      B.enqueue(data++) ;
   }
   //reportSizes(B) ;

   for (int i=1 ; i <= 5201920 ; i++) {
      B.dequeue() ;
   }
   //reportSizes(B) ;

   //cout << "\n-------------------\n" ;
   cout << "Total of 5242870 items added and removed\n" ;


   cerr << "#887134# Reached end of main program.\n" ;
   return 0 ;
}
