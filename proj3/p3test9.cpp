// File: test9.cpp
// 
// Timing test #2
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

   int m = sqrt(n) / 2 ;

   ExpBST T(depth, min_height, factor) ;

   for (int j = 0 ; j < m ; j++) {
    
      int base = 4 * j * m ;

      for(int k = base ; k < base + 4 * m ; k++) {
	 T.insert(k) ;
      }

      for (int k = base + m ; k < base + 2 * m ; k++) {
	 T.remove(k) ;
      }
   }

   report(T) ;
}
