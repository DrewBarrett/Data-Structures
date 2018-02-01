#include "ExpBST.h"
#include <iostream>
#include <math.h>

using namespace std;

ExpBST::ExpBST() {
    m_depth = 0;
    m_minHeight = 0;
    m_factor = 0;
    m_numRebalance = 0;
    m_failedRebalance = 0;
    m_exceedsHeight = 0;
    m_baseNode = NULL;
    #ifdef DEBUG
    cout << "DEBUG: Default ExpBST constructor called!\n";
    #endif
}

ExpBST::ExpBST(int depth, int minHeight, float factor) {
    m_depth = depth;
    m_minHeight = minHeight;
    m_factor = factor;
    m_numRebalance = 0;
    m_failedRebalance = 0;
    m_exceedsHeight = 0;
    m_baseNode = NULL;
    #ifdef DEBUG
    cout << "DEBUG: Non default ExpBST constructor called!\n";
    #endif
}

ExpBST::~ExpBST() {
    // calls recursive function makeEmpty
    makeEmpty(m_baseNode);
}

ExpBST::ExpBST(const ExpBST &other) {
    m_depth = other.m_depth;
    m_minHeight = other.m_minHeight;
    m_factor = other.m_factor;
    m_numRebalance = other.m_numRebalance;
    m_failedRebalance = other.m_failedRebalance;
    m_exceedsHeight = other.m_exceedsHeight;
    //  use binaryNode copy constructor
    if (other.m_baseNode != NULL)
        m_baseNode = new BinaryNode(*other.m_baseNode);
    else
        m_baseNode = NULL;
}

ExpBST& ExpBST::operator=(const ExpBST& rhs) {
    if (this == &rhs)
        return *this;
    makeEmpty(m_baseNode);
    m_depth = rhs.m_depth;
    m_minHeight = rhs.m_minHeight;
    m_factor = rhs.m_factor;
    m_numRebalance = rhs.m_numRebalance;
    m_failedRebalance = rhs.m_failedRebalance;
    m_exceedsHeight = rhs.m_exceedsHeight;
    //  use binaryNode copy constructor
    if (rhs.m_baseNode != NULL)
        m_baseNode = new BinaryNode(*rhs.m_baseNode);
    else
        m_baseNode = NULL;
    return *this;
}

void ExpBST::makeEmpty(BinaryNode * & t) {
    if (t != NULL) {
        // first delete both children then t
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = NULL;
    }
}

bool ExpBST::locate(const char *position, int& key) {
    // call recursive version of function
    return locate(position, key, m_baseNode, 0);
}
bool ExpBST::locate(const char *position, int& key, BinaryNode* t, int pos) {
    if (t == NULL) {
        // if we are null then theres nothing to locate
        return false;
    }
    if (!position[pos]) {
        // we have reached the end of the string so return here
        key = t->m_data;
        return true;
    }
    // we still have instructions to follow
    BinaryNode* next = NULL;
    if (position[pos] == 'L') {
        next = t->left;
    }
    else {
        next = t->right;
    }
    pos++;
    return locate(position, key, next, pos);
}

bool ExpBST::find(int key) {
    // call recursive version
    return find(key, m_baseNode);
}
bool ExpBST::find(int key, BinaryNode *t) {
    if (t == NULL) {
        // we didnt find value
        return false;
    }
    else if (key < t->m_data) {
        return find(key, t->left);
    }
    else if (key > t->m_data) {
        return find(key, t->right);
    }
    else {
        // match
        return true;
    }
}

void ExpBST::insert(int key) {
    // Call recursive version of function 
    insert(key, m_baseNode);
}
int ExpBST::insert(int key, BinaryNode * & t) {
    // This recursive function returns if height and size should be incremented
    int result;
    if (t == NULL) {
        // if we are NULL then we have found insertion spot
        t = new BinaryNode(key);
        // tell previous nodes to update size and height
        return 1;
    }
    else if (key < t->m_data) {
        result = insert(key, t->left);
    }
    else if (key > t->m_data) {
        result = insert(key, t->right);
    }
    else {
        // Duplicate so height and size increases by 0
        result = 0;
    }

    // increment t by result and return result (if we are not a duplicate of course)
    if (result > 0) {
        // if the height is greater than currently we apply
        if (result > t->m_height) {
            t->m_height = result;
        }
        result = t->m_height + 1; // increment result so next guy knows new height
        // our subtree is now one larger
        t->m_size += 1;
    }
    // initiate rebalance if needed
    initiateRebalance(t);
    return result;
}

bool ExpBST::remove(int key) {
    // call recursive version of function
    return remove(key, m_baseNode);
}
bool ExpBST::remove(int key, BinaryNode * & t) {
    if (t == NULL) {
        return false; // we didnt find it
    }
    bool found = false;
    // Traverse until we find the element
    if (key < t->m_data) {
        found = remove(key, t->left);
    }
    else if (key > t->m_data) {
        found = remove(key, t->right);
    }
    else if (t->left != NULL && t->right != NULL) {
        // we have two children
        // find the lowest value on the right and take on its value
        t->m_data = findMin(t->right)->m_data;
        // then delete the node with that value
        found = remove(t->m_data, t->right);
    }
    else {
        //zero or one child
        BinaryNode *oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
        return true;
    }
    // update our size and height based on the fellas beneath us
    // (if there was a change)
    if (found) {
        updateInfo(t);
    }
    initiateRebalance(t);
    return found;
}

void ExpBST::updateInfo(BinaryNode * t) {
    int size = 1;
    int height = 0;
    if (t->left != NULL) {
        // if we have a left node
        size += t->left->m_size;
        height = t->left->m_height + 1;
    }
    if (t->right != NULL) {
        // if we have a right node
        size += t->right->m_size;
        height = (t->right->m_height + 1 > height) ? t->right->m_height + 1 : height;
    }
    t->m_size = size;
    t->m_height = height;
}

void ExpBST::initiateRebalance(BinaryNode * & t) {
    // check if node is considered for rebalance
    if (t->m_height < m_minHeight) {
        return;
    }
    // check if node is not balanced
    // height > factor * log( nodes )
    #ifdef DEBUG
    cout << "DEBUG: Initiating rebalance caleld at height = " << t->m_height << " < " << m_factor * log2(t->m_size) << "\n";
    #endif
    if (float(t->m_height) > m_factor * log2(t->m_size)) {
        rebalance(t);
    }
    if (float(t->m_height) > m_factor * log2(t->m_size)) {
        #ifdef DEBUG
        cout << "DEBUG: Shit didnt work\n";
        #endif
        m_exceedsHeight++;
    }

}

void ExpBST::rebalance(BinaryNode * & t) {
    m_numRebalance++;
    // First phase
    // Find which node should be root of new subtree
    // try every singleton node as the root and determine the optimum
    //  height of both subtrees
    // (call shortestBST)

    /******************
     * Step one is to create the inorder array and pass it to shortestBST
     * ***************/
    // construct list of singletons and subtrees
    //  do inorder walk up to m_depth starting at t 
    // This will be an array of 2^(m_depth + 1) - 1 length (max size)
    //      ie depth of 0 is array of 1
    //      depth of 1 is array of 3
    //      depth of 2 is array of 7
    //      depth of 3 is array of 15 (2^(h+1) - 1 )
    int currDepth = 0;
    BinaryNode** inorder;
    inorder = new BinaryNode*[int(pow(2, m_depth + 1) - 1)];
    int arraySize = 0;
    inOrderToDepth(currDepth, arraySize, inorder, t);
   
    #ifdef DEBUG
    for (int i = 0; i < arraySize; i++) {
        if (inorder[i] == NULL)
            cout << "null ";
        else
            cout << inorder[i]->m_data << " ";
    }
    cout << "DEBUG: Rebalancing at " << t->m_data << endl;
    #endif


    
    int newHeight = 0;
    BinaryNode * newRoot = shortestBST(arraySize, newHeight, inorder);

    #ifdef DEBUG
    for (int i = 0; i < arraySize; i++) {
        if (inorder[i] == NULL)
            cout << "null ";
        else
            cout << inorder[i]->m_data << " ";
    }
    cout << "DEBUG: The best root is " << newRoot->m_data << " with height " << newHeight << endl;
    #endif

   
    if (newHeight >= t->m_height) {
        #ifdef DEBUG
        cout << "DEBUG: Failed to make shorter tree\n";
        #endif
        m_failedRebalance++;
    }
    
    /******************
     * We are now given the new target height and best root node
     ******************/
    
    reconstructBST(arraySize, inorder, newRoot);
    
    t = newRoot;
    delete [] inorder;
}

// Takes a list of inorder nodes, uses shortestBST to find the best root node,
//  and then splits the list of inorder nodes based on that best node.
//  Calls itself for left and right splits 
//  Reassembles BST from bottom up
BinaryNode*& ExpBST::reconstructBST(int length, BinaryNode* nodes[], BinaryNode*& t) {
    // BASE CASE
    BinaryNode*& newOne = t;
    if (t == NULL || length <= 1)
        return newOne;
    #ifdef DEBUG    
    cout << "Entering reconstruct BST at " << t->m_data << endl;
    #endif
    t->right = NULL;
    t->left = NULL;
    /*********
     * Step 1: Split inorder array into two arrays
     *********/
    int leftLength = 0;
    int rightLength = 0;
    for (int i = 0; i < length; i++) {
        if (nodes[i] != NULL && nodes[i]->m_data == t->m_data) {
            leftLength = i;
            #ifdef DEBUG
            cout << "we found " << t->m_data << " at " << leftLength << " " << nodes[i]->m_data << endl;
            #endif
            break;
        }
    }
    rightLength = length - (leftLength + 1);

    BinaryNode** leftNodes;
    leftNodes = new BinaryNode*[leftLength];
    for (int j = 0; j < leftLength; j++) {
        leftNodes[j] = nodes[j];
    }
    BinaryNode** rightNodes;
    rightNodes = new BinaryNode*[rightLength];
    for (int j = 0; j < rightLength; j++) {
        int pos = (leftLength + 1 ) + j;
        rightNodes[j] = nodes[pos];
    }


    /********
     * Step 2: Call shortest BST on those two arrays to get children nodes
     * 
     * Step 3: Call reconstructBST on those two children
     ********/
    int leftHeight = 0;
    int rightHeight = 0;
    // the shortestBST root of left Nodes is our left!
    if (leftLength == 0) {
        t->left = NULL;
        //cout << "fuck me in the asshole";
    }
    else {
        t->left = shortestBST(leftLength, leftHeight, leftNodes);
        #ifdef DEBUG
        cout << "our left was set to ";
        t->left == NULL ? cout << "null" : cout << t->left->m_data;
        cout << endl;
        #endif
        reconstructBST(leftLength, leftNodes, t->left);
    }
        

    if (rightLength == 0) {
        t->right = NULL;
    }  
    else {
        t->right = shortestBST(rightLength, rightHeight, rightNodes);
        #ifdef DEBUG
        cout << "our right was set to ";
        t->right == NULL ? cout << "null" : cout << t->right->m_data;
        cout << endl;
        #endif
        reconstructBST(rightLength, rightNodes, t->right);
    }
        
    
    
    
    delete [] rightNodes;
    delete [] leftNodes;

    /*******
     * Step 4: returning from recursion!
     *          Update our height and size!
     *********/
    updateInfo(newOne);
    #ifdef DEBUG
    cout << "Exiting reconstruct BST at " << t->m_data << endl;
    #endif
    return newOne;
}


// return the node with lowest possible height as root of subtree
// Takes an array of pointers to BinaryNode's, and the length of said array
// Returns best possible height as reference and best root as pointer
/************************
 * Step 1: We are passed an array of singletons and non singletons
 *
 * Step 2: For every item in this list we call ourselves with left subtree options
 *          and right subtree options
 *      This will recursively give us the best height
 *      ( we will compare this to best height of every other possible node 
 *         in the given array also)
 *         HAPPENS ONLY IF THERE IS MORE THAN ONE POSSIBLE SINGLETON
 *
 * Step 3: (BASE CASE) The array we are passed has only one singleton (that is
 *                      not null) so only one subtree
 *                      is possible so we can return the node with best lower height
 *
 * ***********************/
BinaryNode* ExpBST::shortestBST(int length, int& bestHeight, BinaryNode* nodes[]) {
    // BASE CASE
    // tree SINGLETON tree
    if (length < 4) {
        // the height is the height of the biggest subtree +1

        // if there is only a subtree
        if (length == 1) {
            bestHeight = 999;
            if (nodes[0] != NULL)
                bestHeight = nodes[0]->m_height;
            return nodes[0];
        }
        int height = 0;
        
        // if there is a subtree at pos 2
        if (length >= 3 && nodes[2] != NULL)
            height = nodes[2]->m_height;

        // pick the height of the bigger subtree + 1
        if (nodes[0] != NULL) {
            height = nodes[0]->m_height > height ? nodes[0]->m_height : height;
        }

        bestHeight = height + 1;
        // the singleton is the only one
        return nodes[1];
    }

    // Step 2
    // More than one singleton as possibility

    // for every singleton check what the height would be if we were head
    //  (singleton is odd)
    // we do this by finding the best right tree root node and left
    int newHeight = -1;
    BinaryNode* bestSingleton = nodes[0];
    for (int i = 0; i < length; i++) {
        if (i % 2 != 0) {
            // odds are singletons
            if (nodes[i] != NULL) {
                // null can't be our new root
                
                // Find the best root of left subtree
                int leftHeight = 0;
                int leftLength = i;
                if (leftLength > 0) {
                    BinaryNode** leftNodes;
                    leftNodes = new BinaryNode*[leftLength];
                    for (int j = 0; j < leftLength; j++) {
                        leftNodes[j] = nodes[j];
                    }
                    BinaryNode* bestLeft = shortestBST(leftLength, leftHeight, leftNodes);
                    delete [] leftNodes;
                }
                
                // Find the best root of right subtree
                int rightHeight = 0;
                int rightLength = length - i - 1;
                if (rightLength > 0) {
                    BinaryNode** rightNodes;
                    rightNodes = new BinaryNode*[rightLength];
                    for (int j = 0; j < rightLength; j++) {
                        int pos = (i + 1) + j;
                        rightNodes[j] = nodes[pos];
                    }
                    BinaryNode* bestRight = shortestBST(rightLength, rightHeight, rightNodes);
                    delete [] rightNodes;
                }
               
                int currNodeHeight = rightHeight + 1;
                if (leftHeight + 1 > currNodeHeight)
                    currNodeHeight = leftHeight + 1;
                if (newHeight == -1 || currNodeHeight < newHeight) {
                    newHeight = currNodeHeight;
                    bestSingleton = nodes[i];
                }
                
                
            }
        }
    }
    bestHeight = newHeight;
    return bestSingleton;
}

void ExpBST::inOrderToDepth(int currDepth, int& currPos, BinaryNode* inorder[], BinaryNode *t) {
    if (currDepth > m_depth) {
        return;
    }
    if (t != NULL) {
        // First we print the left tree
        currDepth++;
        inOrderToDepth(currDepth, currPos, inorder, t->left);
        currDepth--;
        // Then we print ourselves out
        inorder[currPos] = t;
        currPos++;
        // Then we print out the right tree
        currDepth++;
        inOrderToDepth(currDepth, currPos, inorder, t->right);
        currDepth--;
    }
    if (t == NULL) {
        inorder[currPos] = NULL;
        currPos++;
    }
}


BinaryNode* ExpBST::findMin(BinaryNode *t) {
    if (t == NULL) {
        return NULL; // tree is empty
    }
    if (t->left == NULL) {
        return t;
    }
    else {
        return findMin(t->left);
    }
}

void ExpBST::inorder() {
    // Call recursive version of function
    inorder(m_baseNode);
}

void ExpBST::inorder(BinaryNode *t) {
    if (t != NULL) {
        // First we print the left tree
        cout << "(";
        if (t->left != NULL) {
            inorder(t->left);
        }
        // Then we print ourselves out
        cout << t->m_data << ":" << t->m_height << ":" << t->m_size;
        // Then we print out the right tree
        if (t->right != NULL) {
            inorder(t->right);
        }
        cout << ")";
    }
}


// See piazza #263
//void ExpBST::resetStats() {
//   m_numRebalance = 0;
//    m_failedRebalance = 0;
//    m_exceedsHeight = 0;
//}
