// File: p3test8.cpp
// 
// UMBC CMSC 341 Fall 2017 Project 3
//
// Timing test #1
//
// Use on command line: 
//
//   ./p3test8.out 1500 3 4 2.0
//
// 1500 = # of reps
// 3 = depth
// 4 = smallest height for rebalance
// 2.0 = factor
//

#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std ;

#include "ExpBST.h"


void report(const ExpBST& T) {

   cout << "***** ExpBST Report\n" ;

   cout << "   size = " << T.size() << endl ;
   cout << "   height = " << T.height() << endl ;
   cout << "   height/log(size) = " << ( (float) T.height() ) / log2(T.size()) << endl ;
    
   cout << "   numRebalance = " << T.getNumRebalance() << endl ;
   cout << "   failedRebalance = " << T.getFailedRebalance() << endl ;
   cout << "   exceedsHeight = " << T.getExceedsHeight() << endl ;

   cout << "   maxRebalanceDepth = " << T.getMaxRebalanceDepth() << endl ;
   cout << "   minRebalanceHeight = " << T.getMinRebalanceHeight() << endl ;
   cout << "   rebalanceFactor = " << T.getRebalanceFactor() << endl ;
}


int main(int argc, char* argv[]) {

   if (argc < 4) {
      cout << "Usage: " << argv[0]
           << " reps depth min_height factor.\n" ;
      return 1 ;
   }

   int n = atoi(argv[1]) ;
   int depth = atoi(argv[2]) ;
   int min_height = atoi(argv[3]) ;
   float factor = atof(argv[4]) ;

   ExpBST T(depth, min_height, factor) ;

   for(int k = 1 ; k < 2 * n ; k++) {
      T.insert(k) ;
   }

   for (int k = n/2 ; k < n + n/2 ; k++) {
      T.remove(k) ;
   }

   report(T) ;
}
