#ifndef _MEDIANHEAP_H_
#define _MEDIANHEAP_H_

#include "stdexcept"

// Heap
//  Array based implementation that starts at index 1
template <typename U>
class Heap
{   
public:
    // Default constructor
    //  do no harm

    // constructor
    Heap(int size, bool (*fptr)(const U&, const U&)) {
        // index starts at 1
        m_data = new U[size + 1];
        m_heapSize = 0;
        m_arraySize = size + 1;
        compare = fptr;
    }
    
    Heap(const Heap<U>& other) {
        m_data = new U[other.m_arraySize];
        m_heapSize = other.m_heapSize;
        m_arraySize = other.m_arraySize;
        compare = other.compare;
        for (int i = 1; i <= m_heapSize; i++) {
            m_data[i] = other.m_data[i];
        }
    }


    // Destructor
    ~Heap() {
        delete [] m_data;
    }

    // getRoot
    //  returns the root item in the heap
    U getRoot() {
        if (isEmpty())
            throw string("Heap is empty");
        else
            return m_data[1];
    }

    // isEmpty
    bool isEmpty() {
        return (m_heapSize == 0);
    }

    int size() {
        return m_heapSize;
    }
    // percolate up
    void percolateUp(int nodeIndex) {
        int parentIndex;
        U tmp;
        if (nodeIndex != 1) {
            parentIndex = getParentIndex(nodeIndex);
            // if this is a minHeap this is GU
            // if its a max heap then LU
            if (compare(m_data[parentIndex], m_data[nodeIndex])) {
                tmp = m_data[parentIndex];
                m_data[parentIndex] = m_data[nodeIndex];
                m_data[nodeIndex] = tmp;
                percolateUp(parentIndex);
            }
        }

    }

    // insert
    void insert(U value) {
        if (m_heapSize == m_arraySize)
            throw string("Heap's underlying storage is overflow");
        else {
            m_heapSize++;
            m_data[m_heapSize] = value;
            percolateUp(m_heapSize);
        }
    }
    
    // percolateDown
    void percolateDown(int nodeIndex) {
        int leftChildIndex, rightChildIndex, minIndex;
        U tmp;
        leftChildIndex = getLeftChildIndex(nodeIndex);
        rightChildIndex = getRightChildIndex(nodeIndex);
        if (rightChildIndex > m_heapSize) {
            if (leftChildIndex > m_heapSize)
                return;
            else
                minIndex = leftChildIndex;
        } else {
            // oposite of compare
            // if min heap LU=
            // if max heap GU=
            if (!compare(m_data[leftChildIndex], m_data[rightChildIndex]))
                minIndex = leftChildIndex;
            else
                minIndex = rightChildIndex;
        }
        // if this is a minHeap this is GU
        // if its a max heap then LU
        if (compare(m_data[nodeIndex], m_data[minIndex])) {
            tmp = m_data[minIndex];
            m_data[minIndex] = m_data[nodeIndex];
            m_data[nodeIndex] = tmp;
            percolateDown(minIndex);
        }
    }

    // remove min
    void removeRoot() {
        if (isEmpty())
            throw string("Heap is empty");
        else {
            m_data[1] = m_data[m_heapSize];
            m_heapSize--;
            if (m_heapSize > 0)
                percolateDown(1);
        }
    }

    bool deleteItem(U& givenItem, bool (*equalUo) (const U&, const U&)) {
        for (int i = 1; i <= m_heapSize; i++) {
            if (equalUo(m_data[i], givenItem)){
                givenItem = m_data[i];
                removeIndex(i);
                return true;
            }
        }
        return false;
    }

    void dump() {
        cout << "size = " << m_heapSize << ", capacity = " << m_arraySize
            << endl;
        for (int i = 1; i <= m_heapSize; i++) {
            cout << "Heap[" << i << "] = (" << m_data[i] << ")\n";
        }
    }

     // Return copy of item in pos (starting at index 1)
    //  pos invalid throw out_of_range
    U locate(int pos) {
        if (pos < 1 || pos > m_heapSize)
            throw out_of_range("pos out of range");
        return m_data[pos];
    }

    U findCompare(bool (*fptr) (const U&, const U&)) {
        U best = m_data[1];
        for (int i = 1; i <= m_heapSize; i++) {
            if (fptr(m_data[i], best))
                best = m_data[i];
        }
        return best;
    }

private:
    U *m_data;
    int m_heapSize; // lol guess what this is
    int m_arraySize;

    // the compare function
    bool (*compare) (const U&, const U&);

    int getLeftChildIndex(int nodeIndex) {
        return 2 * nodeIndex;
    }

    int getRightChildIndex(int nodeIndex) {
        return 2 * nodeIndex + 1;
    }

    int getParentIndex(int nodeIndex) {
        return (nodeIndex) / 2;
    }

    void removeIndex(int i) {
        m_data[i] = m_data[m_heapSize];
        m_heapSize--;
        if (m_heapSize > 0) {
            percolateDown(i);
            percolateUp(i);
        }
    }

};

// Median Heap
// This bad boy has TWO heaps
template <typename T>
class MedianHeap
{
public:

    // Constructor
    // First two parameters are function pointers that compare items of type t
    //  First function = less than
    //  Second function = greater thana
    // Creates a MedianHeap that can hold cap items (usable immediately)
    MedianHeap( bool (*lt) (const T&, const T&),
                bool (*gt) (const T&, const T&), int cap=100 )
    {
        int heapSize = (cap / 2) + 1;
        m_minHeap = new Heap<T>(heapSize, gt);
        m_maxHeap = new Heap<T>(heapSize, lt);
        lessThan = lt;
        greaterThan = gt;
        m_capacity = cap;
    }

    // Copy Constructor
    //template <typename T2>
    MedianHeap(const MedianHeap<T>& other) {
        m_minHeap = new Heap<T>(*other.m_minHeap);
        m_maxHeap = new Heap<T>(*other.m_maxHeap);

        m_maximum = other.m_maximum;
        m_minimum = other.m_minimum;

        lessThan = other.lessThan;
        greaterThan = other.greaterThan;

        m_capacity = other.m_capacity;

    }

    // Destructor
    ~MedianHeap() {
        delete m_minHeap;
        delete m_maxHeap;
    }

    // Assingment operator
    //template <typename T2>
    const MedianHeap<T>& operator=(const MedianHeap<T>& rhs) {
        if (this == &rhs)
            return *this;
        delete m_minHeap;
        delete m_maxHeap;

        m_minHeap = new Heap<T>(*rhs.m_minHeap);
        m_maxHeap = new Heap<T>(*rhs.m_maxHeap);

        m_maximum = rhs.m_maximum;
        m_minimum = rhs.m_minimum;

        lessThan = rhs.lessThan;
        greaterThan = rhs.greaterThan;

        m_capacity = rhs.m_capacity;

        return *this;
    }

    // size
    //  Returns total number of items in constant time
    int size() {
        return m_minHeap->size() + m_maxHeap->size();
    }

    bool empty() {
        return size() == 0;
    }

    // capacity
    //  Returns max number of items stored in MedianHeap in constant time
    int capacity() {
        return m_capacity;
    }

    // insert
    //  Adds the item to the median heap
    //  if capacity is reached then throw out_of_range
    //  Must run in O(log n) time
    //  - If item is smaller than the median we put it in the max heap
    //      Otherwise we put it in the min heap
    //  - If the size of minHeap and maxHeap differ by more than one
    //      we need to rebalance
    //  - compare inserted value to see if its the new min or max
    void insert(const T& item) {
        // if we are emtpy throw it in the minHeap
        if (!empty()) {
            T median = getMedian();
            // if item is smaller we put it in the max heap
            if (lessThan(item, median)) {
                m_maxHeap->insert(item);
            }
            else
                m_minHeap->insert(item);

            // check if we are new min or max
            if (lessThan(item, m_minimum))
                m_minimum = item;
            else if (greaterThan(item, m_maximum))
                m_maximum = item;
        }
        else {
            // If we are empty
            m_maxHeap->insert(item);
            m_maximum = item;
            m_minimum = item;
        }
        checkRebalance();
    }

    

    /************
     * GETTERS
     *  
     *  Returns a copy of the object with the median min or max      
     *  Returns out_of_range if median heap is empty
     *  Run in constant time
     * 
     ************/

    // return either one of the two objects closest to the middle
    //  (the bigger one has the median if they arent the same size)
    T getMedian() {
        if (empty())
            throw out_of_range("MedianHeap is empty");
        if (m_minHeap->size() > m_maxHeap->size()) {
            return m_minHeap->getRoot();
        }
        return m_maxHeap->getRoot();
    }
    T getMin() {
        if (empty())
            throw out_of_range("MedianHeap is empty");
        return m_minimum;
    }
    T getMax() {
        if (empty())
            throw out_of_range("MedianHeap is empty");
        return m_maximum;
    }
    
    // deleteItem
    //  Looks for an item in the median heap equal to givenItem according to
    //      equalTo()
    //  - do not use key
    //  givenItem does not have all valid entries so check both heaps
    //  Returns true if item found and copies values of item
    //  Returns false and makes no modifications
    //  If empty throw out_of_range
    //  Runs in O(n)
    //  - If the size of the heaps differs by more than one we rebalance
    //  - If we removed the min or max we need to find the new one lol
    bool deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&)) {
        bool status = m_maxHeap->deleteItem(givenItem, equalTo);
        if (!status) 
            status = m_minHeap->deleteItem(givenItem, equalTo);
        // TODO min and max
        checkRebalance();
        if (size() > 0) {
            if (equalTo(givenItem, m_minimum)) {
                if (m_maxHeap->size() > 0)
                    m_minimum = m_maxHeap->findCompare(lessThan);
                else
                    m_minimum = m_minHeap->getRoot();
            }
            if (equalTo(givenItem, m_maximum)) {
                if (m_minHeap->size() > 0)
                    m_maximum = m_minHeap->findCompare(greaterThan);
                else
                    m_maximum = m_maxHeap->getRoot();
            }
        }
        
        return status;
    }

    // Dump
    //  prints out contents
    //  does not modify
    void dump() {
        cout << "... MedianHeap::dump() ...\n\n";
        cout << "------------Max Heap--------------\n";
        m_maxHeap->dump();
        cout << "------------Min Heap--------------\n";
        m_minHeap->dump();
        cout << "----------------------------------\n\n";
        cout << "min\t= " << m_minimum << endl;
        cout << "median\t= " << getMedian() << endl;
        cout << "max\t= " << m_maximum << "\n";
    }

    /*************
     * Grading Functions
     ***********/

    // These two functions added together should equal size()
    int maxHeapSize() {
        return m_maxHeap->size();
    }
    int minHeapSize() {
        return m_minHeap->size();
    }
    
    // Return copy of item in pos (starting at index 1)
    //  pos invalid throw out_of_range
    T locateInMaxHeap(int pos) {
        return m_maxHeap->locate(pos);
    }
    T locateInMinHeap(int pos) {
        return m_minHeap->locate(pos);
    }

private:
    Heap<T>* m_minHeap;
    Heap<T>* m_maxHeap;

    T m_maximum; // The biggest value currently present
    T m_minimum; // The smallest value currentyly present

    bool (*lessThan)(const T&, const T&);
    bool (*greaterThan)(const T&, const T&);

    int m_capacity;

    void checkRebalance() {
        int diff = m_minHeap->size() - m_maxHeap->size();
        if (diff > 1 || diff < -1)
            rebalance();
    }

    // Move the root of the larger heap and insert it in the other
    void rebalance() {
        Heap<T> *larger = NULL;
        Heap<T> *smaller = NULL;
        
        // assign the larger and smaller heaps
        if (m_minHeap->size() > m_maxHeap->size()) {
            larger = m_minHeap;
            smaller = m_maxHeap;
        } else {
            larger = m_maxHeap;
            smaller = m_minHeap;
        }
        
        T item = larger->getRoot();
        larger->removeRoot();
        smaller->insert(item);
    }
};

#endif /* _MEDIANHEAP_H_ */
