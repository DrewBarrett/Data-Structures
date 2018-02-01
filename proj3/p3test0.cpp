// File: p3test0.cpp
// 
// UMBC CMSC 341 Fall 2017 Project 3
//
// Simpler test of insertion ( for unbalanced binary tree )
//
// should produce
// (((((3:0:1)7:2:4((9:0:1)11:1:2))14:3:8((15:1:2(17:0:1))20:2:3))
//    22:4:13(((24:0:1)26:1:2)30:2:4(37:0:1)))41:5:22((((50:0:1)
//    54:1:3(59:0:1))60:2:4)64:3:8((71:1:2(75:0:1))79:2:3)))
//
//  true 26 = 26
//  true 75 = 75
//  false -69 = -69
//  true 41 = 41
#include <iostream>
#include <cmath>
using namespace std;

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



int main() {

   ExpBST T(3, 3, 2.0) ;

   int toInsert[22] = {41,22,14,7,3,11,9,20,15,17,30,26,24,37,64,60,54,50,
                            59,79,71,75};
    for (int i = 0; i < 22; i++)
    {
        T.insert(toInsert[i]);
    }
    T.inorder();

    ExpBST ass = T;
    ExpBST copy(T);

    int key = -69;
    cout << T.locate("LRL", key) << " 26 = ";
    cout << key << endl;
    cout << T.locate("RRLR", key) << " 75 = ";
    cout << key << endl;
    key = -69;
    cout << T.locate("RLR", key) << " -69 = ";
    cout << key << endl;
    cout << T.locate("", key) << " 41 = ";
    cout << key << endl;

    cout << "Find 0 false = " << T.find(0) << endl;
    cout << "Find 41 true = " << T.find(41) << endl;

    T.remove(75); 
    T.inorder(); cout << endl;
    T.remove(64);
    T.inorder(); cout << endl;
    ass = T;
    T.remove(41);
    T.inorder(); cout << endl;
    cout << "Copied: \n";
    copy.inorder(); cout << endl;
    cout << "Assigned: \n";
    

    ass.inorder(); cout << endl;


   cout << endl << endl ;
   report(T) ;

}
