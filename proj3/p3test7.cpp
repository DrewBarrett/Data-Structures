// File: p3test7.cpp
//
// UMBC CMSC 341 Fall 2017 Project 3
//
// Big test with recursive sanityCheck() and lots of data.
//

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <set>
using namespace std ;

#include "ExpBST.h"

const int depthLimit = 50 ;


// This function recursively checks if a ExpBST is correct, by checking:
//
//   1. keys are in order
//   2. left subtree is not more than twice the size of right subtree
//      or vice versa
//
// This function relies on locate() member function working correctly.
//
// Parameters:  
//   ExpBST& T  = tree to be checked, passed by reference
//   char pos[]  = must be pre-allocated with depthLimit chars
//   int& key    = stores key in node indicated by pos, if it exists
//   int& height = stores height of node indicated by pos, if it exists
//   int& size   = stores size of node indicated by pos, if it exists
//   bool report = give report for current node? defaults to true.
//
// Return value:
//    true if T has a node at pos
//    false if T does not have a node at pos
//
// Notes: 
// - if return value is false, parameters key, height and size are
//   not changed.
// - The pos string/array essentially acts as a stack for exploration
//   of the ExpBST. It remembers where we've been and is used to
//   determine where we have to check next.
//

bool sanityCheck(ExpBST& T, char pos[], int depth, int& key, int& height, int& size, bool report=true) {

   int leftKey, rightKey ;
   int leftHeight=-1, rightHeight=-1 ;
   int leftSize=0, rightSize=0 ;
   bool hasLeft, hasRight ;


   // Try to catch bad BST with cycles
   //
   if (depth >= depthLimit) {
      throw out_of_range("Depth limit reached. Something looks wrong!\n") ;
   }


   // Is does current position have a node?
   //
   if ( ! T.locate(pos, key) ) return false ;


   // Add extra '\0' so pos string can be extended
   //
   pos[depth+1] = '\0' ;


   // Recursively checks left subtree.
   //
   pos[depth] = 'L' ;
   hasLeft= sanityCheck(T, pos, depth+1, leftKey, leftHeight, leftSize, report) ;


   // Recursively checks right subtree.
   //
   pos[depth] = 'R' ;
   hasRight= sanityCheck(T, pos, depth+1, rightKey, rightHeight, rightSize, report) ;


   pos[depth] = '\0' ;  // restores pos[]


   // Compute current node's height and size
   //
   height = 1 + ( (leftHeight > rightHeight) ? leftHeight : rightHeight ) ; 
   size = 1 + leftSize + rightSize ;


   // Check key ordering for left child
   //
   if (hasLeft && leftKey >= key) {
      cerr << "\nIn position " << pos 
           << " (key=" << key << ",height=" << height << ",size=" << size << ")"
           << " left child's key not less than current node's key:"
	   << leftKey << " " << key << endl ;
   }


   // Check key ordering for right child
   //
   if (hasRight && rightKey <= key) {
      cerr << "\nIn position " << pos 
           << " (key=" << key << ",height=" << height << ",size=" << size << ")"
           << " right child's key not greater than current node's key:"
	   << rightKey << " " << key << endl ;
   }


   // Check height <= RebalanceFactor * log2(size)
   //
   float factor =  T.getRebalanceFactor() ;

   if (height > T.getMinRebalanceHeight() ) {
      if ( height > factor * log2(size) ) {
	 cerr << "\nIn position " << pos 
              << " (key=" << key << ",height=" << height << ",size=" << size << ")"
	      << " tree too tall, " << factor <<  "* log(size) = "
	      << factor * log2(size) << endl ;
      }
   }


   // Give stats for current node, if so desired.
   //

   if (report) {
      cout << "\nNode report on position " << pos << " :" << endl ;
      cout << "   key = " << key 
	   << "   height = " << height 
	   << "   size = " << size 
	   << endl ;

      if (hasLeft) {
	 cout << "   left child key = " << leftKey << endl ;
      } else {
	 cout << "   no left child\n" ;
      }

      if (hasRight) {
	 cout << "   right child key = " << rightKey << endl ;
      } else {
	 cout << "   no right child\n" ;
      }
   }

   return true ;

}


bool checkAgainstSTLSet (ExpBST& T, set<int>& S) {

   if (T.size() != S.size() ) {
      cout << "\nError: sizes mismatched:\n" ;
      cout << "   T.size() = " << T.size() << endl ;
      cout << "   S.size() = " << S.size() << endl ;
      return false ;
   }
   
   set<int>::iterator it ;
   const int *ptr ;
   for (it = S.begin() ; it != S.end() ; it++) {
      if (! T.find(*it) ) {
         cout << "\nError: " << *it << " in S but not in T.\n" ;
	 return false ;
      }
   }
   return true ;
}


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
//   printf("   rebalanceFactor = %f\n", rebalanceFactor) ;

}


int main() {

   ExpBST T(3,5,1.5) ;
   set<int> S ;

   // add a bunch of numbers
   //
   T.insert(70) ;
   T.insert(30) ;
   T.insert(110) ;
   T.insert(40) ;
   T.insert(20) ;
   T.insert(41) ;
   T.insert(31) ;
   T.insert(32) ;
   T.insert(33) ;
   T.insert(19) ;
   T.insert(34) ;
   T.insert(15) ;
   T.insert(14) ;
   T.insert(38) ;
   T.insert(81) ;
   T.insert(95) ;
   T.insert(43) ;
   T.insert(17) ;

   S.insert(70) ;
   S.insert(30) ;
   S.insert(110) ;
   S.insert(40) ;
   S.insert(20) ;
   S.insert(41) ;
   S.insert(31) ;
   S.insert(32) ;
   S.insert(33) ;
   S.insert(19) ;
   S.insert(34) ;
   S.insert(15) ;
   S.insert(14) ;
   S.insert(38) ;
   S.insert(81) ;
   S.insert(95) ;
   S.insert(43) ;
   S.insert(17) ;

   T.inorder() ; cout << endl ;
   

   char pos[depthLimit] ;
   pos[0] = '\0' ;
   int key, height, size ;

   // Do check
   //
   cout << "First a small test...\n" ;
   sanityCheck(T,pos,0,key,height,size) ;
   cout << "\n\nSmall tree has root with key=" << key
        << ", height=" << height 
	<< ", size=" << size 
	<< endl; 


   if ( checkAgainstSTLSet(T,S) )  {
      cout << "Passed check against STL set S\n" ;
   } else {
      cout << "*** Failed check against STL set S\n" ;
   }

   report(T) ;


   cout << endl << endl ;

   cout << "Now a big test...\n" ;
   
   //ExpBST::resetStats() ;

   ExpBST T2(3,5,2.0) ;
   set<int> S2 ;

   for (int i=1000 ; i<1500 ; i++) {
      T2.insert(i) ;
      S2.insert(i) ;
   }
   for (int i=1200 ; i<1300 ; i++) {
      T2.remove(i) ;
      S2.erase(i) ;
   }
   for (int i=250 ; i<900 ; i++) {
      T2.insert(i) ;
      S2.insert(i) ;
   }
   for (int i=450 ; i<650 ; i++) {
      T2.remove(i) ;
      S2.erase(i) ;
   }
   for (int i=3000 ; i>1600 ; i--) {
      T2.insert(i) ;
      S2.insert(i) ;
   }
   for (int i=2700 ; i>2300 ; i--) {
      T2.remove(i) ;
      S2.erase(i) ;
   }
   for (int i=3500 ; i<6000 ; i++) {
      T2.insert(i) ;
      S2.insert(i) ;
   }
   for (int i=4200 ; i<4750 ; i++) {
      T2.remove(i) ;
      S2.erase(i) ;
   }

   sanityCheck(T2,pos,0,key,height,size,false) ;
   cout << "\n\nBig tree has root with key=" << key
        << ", height=" << height 
	<< ", size=" << size 
	<< endl; 


   if ( checkAgainstSTLSet(T2,S2) )  {
      cout << "Passed check against STL set S2\n" ;
   } else {
      cout << "*** Failed check against STL set S2\n" ;
   }

   report(T2) ;

}
