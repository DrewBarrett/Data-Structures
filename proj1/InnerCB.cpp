#include "InnerCB.h"
#include <iostream>
#include <stdexcept>

using namespace std;

InnerCB::InnerCB(int n){
    // Initialize data members
    m_capacity = n;
    m_size = 0;
    m_start = 0;
    m_end = 0;

    // Allocate memory for n capacity
    m_buffer = new int [m_capacity];

}

InnerCB::InnerCB(const InnerCB& other) {
    // Copy data members
    m_capacity = other.m_capacity;
    m_size = other.m_size;
    m_start = other.m_start;
    m_end = other.m_end;

    // allocate memory for n capacity

    m_buffer = new int [m_capacity];
    for (int i = 0; i < m_size; i++) {
        int pos = m_start + i;
        // if pos is outside we loop
        if (pos >= m_capacity) {
            pos -= m_capacity;
        }
        m_buffer[pos] = other.m_buffer[pos];
    }
}

InnerCB::~InnerCB() {
    // clear allocated memory
    delete [] m_buffer;
}

void InnerCB::enqueue(int data) {
    
    if (InnerCB::isFull()) {
        //throw a overflow error
        throw overflow_error("Buffer is full");
    }
    
    if (!InnerCB::isEmpty()) {
        // if the buffer is empty we will want to start at zero
        //    otherwise we increment the end
        m_end++;

        // if we are at the end of the array we loop
        if (m_end == m_capacity) {
            m_end = 0;
        }
    }

    // set the data at the new end to data
    //cout << "DEBUG: Setting data at " << m_end << " to " << data << endl;
    m_buffer[m_end] = data;

    m_size++;
}

int InnerCB::dequeue() {

    if (InnerCB::isEmpty()) {
        throw underflow_error("Buffer is empty");
    }

    

    // store the oldest item
    int item = m_buffer[m_start];

    // remove the oldest item
    m_start++;
    if (m_start == m_capacity) {
        // if we hit the front of the buffer we loop backwards
        m_start = 0;
    }

    //cout << "DEBUG: We are dequeueing " << item << endl;
    m_size--;
    return item;
}

bool InnerCB::isFull() {
    // if the number of items has reached capacity then we're full
    return m_size >= m_capacity;
}

bool InnerCB::isEmpty() {
    // if the number of items is 0 we are empty
    return m_size <= 0;
}

int InnerCB::capacity() {
    return m_capacity;
}

int InnerCB::size() {
    return m_size;
}

const InnerCB& InnerCB::operator=(const InnerCB& rhs) {
    // self-assignment check
    if (this != &rhs) {
        // delete our current dynamic mem
        delete [] m_buffer;
        
        //copy variables from rhs
        m_capacity = rhs.m_capacity;
        m_size = rhs.m_size;
        m_start = rhs.m_start;
        m_end = rhs.m_end;

        //allocate new mem and copy buffer members
        m_buffer = new int [m_capacity];
        for (int i = 0; i < m_size; i++) {
            int pos = m_start + i;
            // if pos is outside we loop
            if (pos >= m_capacity) {
                pos -= m_capacity;
            }
            m_buffer[pos] = rhs.m_buffer[pos];
        }
    }

    return *this;
}

void InnerCB::dump() {
    cout << "Inner CB dump(): m_size = " << m_size << endl;
    
    // loop over buffer and print out values
    for (int i = 0; i < m_size; i++) {
        int pos = m_start + i;
        // if pos is outside we loop
        if (pos >= m_capacity) {
            pos -= m_capacity;
        }

        cout << "[" << pos << "] " << m_buffer[pos] << ", ";
    }
    cout << endl;
}

