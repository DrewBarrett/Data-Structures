#ifndef EXPBST_H
#define EXPBST_H

#include <cstddef>
struct BinaryNode
{
    int m_data; // guess
    int m_height; // height of node
    int m_size; // size of subtree with this node as head
    BinaryNode *left; // left child
    BinaryNode *right; // right child

    BinaryNode(int data) {
        m_data = data;
        m_height = 0;
        m_size = 1; // if we are root of subtree
        left = NULL;
        right = NULL;
    }
    BinaryNode(const BinaryNode &other) {
        m_data = other.m_data;
        m_height = other.m_height;
        m_size = other.m_size;
        left = NULL;
        right = NULL;
        // if nodes arent null call copy constructor
        if (other.left != NULL)
            left = new BinaryNode(*other.left);
        if (other.right != NULL)
            right = new BinaryNode(*other.right);
    }
};
class ExpBST
{
public:

    // Default constructor
    ExpBST();
    
    // Depth specifies maximum depth taken by the truncated inorder walk
    // minHeight indicates the height of the shortest tree that will be considered for rebalancing
    // factor is the multiple of log m we use to define when the subtree is unbalanced
    ExpBST(int depth, int minHeight, float factor);
    
    // Getters
    int getMaxRebalanceDepth() const { return m_depth; }
    int getMinRebalanceHeight() const { return m_minHeight; }
    float getRebalanceFactor() const { return m_factor; }
    
    // Copy Constructor
    //  Must make a deep copy and create a new object with its own memory
    ExpBST(const ExpBST &other);
    
    // Assignment operator
    //  Deallocates host memory and makes a deep copy
    ExpBST& operator=(const ExpBST& rhs);
    
    // Destructor
    ~ExpBST();

    // Add an item to ExpBST
    //  Runs in time proportional to height
    //  No duplicates
    //  If duplicate then rebalance if needed
    void insert(int key);

    // Removes an item with given key value
    //  Returns boolean that indicates if key was found
    //  Runs in time proportional to height
    bool remove(int key);

    // Returns if key is found
    //  Runs in time proportional to height
    bool find(int key);

    // Rebalances a subtree
    // O(c)
    void rebalance(BinaryNode * & t);

    // Do inorder walk and print the key
    // print key:height:size
    // print ( on left subtree and ) on right
    // print nothing on empty tree
    void inorder();

    // returns whether there is a node in a position
    // position given by L and R
    bool locate(const char *position, int& key);

    // Resets stats related to rebalancing
    //void resetStats();
    static void resetStats() {};

    // Returns number of calls to rebalance
    int getNumRebalance() const { return m_numRebalance; }
    // Gets number of calls to rebalance that did not result in a shorter tree
    int getFailedRebalance() const { return m_failedRebalance; }
    // Gets number of calls to rebalance that resulted in being too tall
    int getExceedsHeight() const { return m_exceedsHeight; }

    // Getters for tree 
    // O(1)
    bool empty() const { return m_baseNode == NULL; }
    int height() const { return m_baseNode != NULL ? m_baseNode->m_height : -1; }
    int size() const { return m_baseNode != NULL ? m_baseNode->m_size : 0; }
private:
    /**********
     * FUNCTIONS
     **********/
    // recursive component of find
    bool find(int key, BinaryNode *t);
    // recursive component of remove
    bool remove(int key, BinaryNode * & t);
    // recursive component of insert
    int insert(int key, BinaryNode * & t);

    // recursive component of inorder
    void inorder(BinaryNode *t);

    // recursive component of destructor
    void makeEmpty(BinaryNode * & t);

    // recursive component of locate
    bool locate(const char *position, int& key, BinaryNode* t, int pos);

    // Recurse all the way down the left 
    BinaryNode* findMin(BinaryNode *t);

    // Initiate rebalancing if node is unbalanced
    void initiateRebalance(BinaryNode * & t);

    // Find the root node of the shortest bst
    BinaryNode* shortestBST(int length, int& bestHeight, BinaryNode* nodes[]);
    //BinaryNode*& shortestBST(BinaryNode * & t, int height);

    // Reconstruct BST based on array of nodes
    BinaryNode*& reconstructBST(int length, BinaryNode* nodes[], BinaryNode*& t);

    // Do an in order traversal to depth
    // returns final size of array
    void inOrderToDepth(int currDepth, int& currPos, BinaryNode* inorder[], BinaryNode* t);

    // updates height and size of a node
    void updateInfo(BinaryNode *t);
    /********
     * VARIABLES
     ********/

    // specifies maximum depth taken by the truncated inorder walk
    int m_depth;

    // indicates the height of the shortest tree that will be considered for rebalancing
    int m_minHeight;

    // the multiple of log m we use to define when the subtree is unbalanced
    float m_factor;

    // Rebalance statistics
    int m_numRebalance;
    int m_failedRebalance;
    int m_exceedsHeight;

    BinaryNode *m_baseNode;
};
#endif
