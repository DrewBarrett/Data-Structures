#include "CBofCB.h"
#include <iostream>
#include <stdexcept>

using namespace std;

CBofCB::CBofCB() {
    m_obSize = 0;
    m_oldest = -1;
    m_newest = -1;
    //m_buffers = NULL;
}

CBofCB::CBofCB(const CBofCB& other) {
    // Copy data members
    m_obSize = other.m_obSize;
    m_oldest = other.m_oldest;
    m_newest = other.m_newest;

    // allocate memory for n capacity

    for (int i = 0; i < m_obSize; i++) {
        int pos = m_oldest + i;
        // if pos is outside we loop
        if (pos >= m_obCapacity) {
            pos -= m_obCapacity;
        }
        // use innercb copy constructor
        m_buffers[pos] = new InnerCB(*other.m_buffers[pos]);
    }
}

CBofCB::~CBofCB() {
    for (int i = 0; i < m_obSize; i++) {
        int pos = i + m_oldest;
        if (pos >= m_obCapacity) {
            pos -= m_obCapacity;
        }
        delete m_buffers[pos];
        m_buffers[pos] = NULL;
    }
}

void CBofCB::enqueue(int data) {
    if (CBofCB::isEmpty()) {
        // if we have no inner buffers we make one
        m_buffers[0] = new InnerCB();
        m_oldest = 0;
        m_newest = 0;
        m_obSize++;
    }

    if (m_buffers[m_newest]->isFull()) {
        // if the newest buffer is full then we need a new one double the size
        if (CBofCB::isFull()) {
            throw overflow_error("Buffer of buffers is full");
        }
        int size = m_buffers[m_newest]->capacity() * 2;
        
        // increment m_newest and loop if needed
        m_newest++;
        if (m_newest == m_obCapacity) {
            m_newest = 0;
        }
        m_buffers[m_newest] = new InnerCB(size);
        m_obSize++;
    }

    m_buffers[m_newest]->enqueue(data);

}

int CBofCB::dequeue() {
    if (CBofCB::isEmpty()) {
        throw underflow_error("The buffer of buffers has no buffers");
    }
    
    int val = m_buffers[m_oldest]->dequeue();

    // if the oldest buffer is now empty and not the last one we deallocate it
    if (m_obSize > 1 && m_buffers[m_oldest]->isEmpty()) {
        delete m_buffers[m_oldest];
        m_buffers[m_oldest] = NULL;
        m_oldest++;
        if (m_oldest == m_obCapacity) {
            m_oldest = 0;
        }
        m_obSize--;
    }

    return val;

}

bool CBofCB::isFull() {
    // if we have all possible buffers and the newest is full
    return m_obSize == m_obCapacity && m_buffers[m_newest]->isFull();
}

bool CBofCB::isEmpty() {
    int size = 0;
    for (int i = 0; i < m_obSize; i++) {
        int pos = i + m_oldest;
        if (pos >= m_obCapacity) {
            pos -= m_obCapacity;
        }
        size += m_buffers[pos]->size();
    }
    return size <= 0;
}

int CBofCB::size() {
    int size = 0;
    for (int i = 0; i < m_obSize; i++) {
        int pos = i + m_oldest;
        if (pos >= m_obCapacity) {
            pos -= m_obCapacity;
        }
        size += m_buffers[pos]->size();
    }
    return size;
}

const CBofCB& CBofCB::operator=(const CBofCB& rhs) {
    // self-assignment check
    if (this != &rhs) {
        // delete our current dynamic mem
        for (int i = 0; i < m_obSize; i++) {
            int pos = i + m_oldest;
            if (pos >= m_obCapacity) {
                pos -= m_obCapacity;
            }
            delete m_buffers[pos];
            m_buffers[pos] = NULL;
        }
        
        //copy variables from rhs
        m_obSize = rhs.m_obSize;
        m_oldest = rhs.m_oldest;
        m_newest = rhs.m_newest;

        //allocate new mem and copy buffer members
        for (int i = 0; i < m_obSize; i++) {
            int pos = m_oldest + i;
            // if pos is outside we loop
            if (pos >= m_obCapacity) {
                pos -= m_obCapacity;
            }
            m_buffers[pos] = new InnerCB(*rhs.m_buffers[pos]);
        }
    }

    return *this;
}

void CBofCB::dump() {
    cout << "-----------------------------------------\n";
    cout << "Outer Circular buffer dump(), m_obsize = " << m_obSize << ":\n";
    // iterate over InnerCB's and call dump
    for (int i = 0; i < m_obSize; i++) {
        int pos = i + m_oldest;
        if (pos >= m_obCapacity) {
            pos -= m_obCapacity;
        }
        cout << "[" << pos << "] ";
        m_buffers[pos]->dump();
    }
    cout << "-----------------------------------------\n";
}