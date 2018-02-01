// File: p1grade01.cpp
//
// Testing InnerCB functionality
//

#include <iostream>
using namespace std ;

#include "InnerCB.h"
#include <list>

int main() {

   int result ;
   InnerCB B(23) ;
   list<int> L ;

   int value=1 ;

   for (int i=0 ; i < 19 ; i++) {
      B.enqueue(value) ;
      L.push_back(value) ;
      value++ ;
   }

   // remove 10 
   for (int i=0 ; i < 10 ; i++) {
      result = B.dequeue() ;  
      L.pop_front() ;
   }

   // add 9 
   for (int i=0 ; i < 9 ; i++) {
      B.enqueue(value) ;
      L.push_back(value) ; 
      value++ ; 
   }

   // remove 10 
   for (int i=0 ; i < 10 ; i++) {
      result = B.dequeue() ;  
      L.pop_front() ;
   }


   // End should wrap
   B.dump() ;


   // remove 20 
   for (int i=0 ; i < 5 ; i++) {
      result = B.dequeue() ;  
      L.pop_front() ;
   }

   // Front should wrap
   B.dump() ;


   int errors = 0 ;

   if ( B.size() == L.size() ) {
      cerr << "#887134##X   ==  -0: InnerCB size is correct.\n" ;
   } else {
      cerr << "#887134##X   ==  -1: InnerCB size is incorrect.\n" ;
   }
   
   while ( !L.empty() && !B.isEmpty() ) {
      result = B.dequeue() ;
      if ( result != L.front() ) {
         errors++ ;
      }
      L.pop_front() ;
   }

   if (errors == 0) {
      cerr << "#887134##X   ==  -0: InnerCB items ordered correctly.\n" ;
   } else if (errors <= 2) {
      cerr << "#887134##X   ==  -1: InnerCB has " << errors << " incorrect  items.\n" ;
   } else {
      cerr << "#887134##X   ==  -2: InnerCB has " << errors << " incorrect  items.\n" ;
   }

   cerr << "#887134# Reached end of main program.\n" ;
   return 0 ;
}
