#include <iostream>
#include "MutablePriorityQueue.h"

MutablePriorityQueue::MutablePriorityQueue() {
    H.push_back(NULL);
    // indices will be used starting in 1
    // to facilitate parent/child calculations
}

bool MutablePriorityQueue::empty() {
    return H.size() == 1;
}

Vertex* MutablePriorityQueue::extractMin() {
    Vertex* x = H[1];
    x->queueIndex = 0;
    H[1] = H.back();
    H.pop_back();
    if ( ! empty())
        heapifyDown(1);
    return x;
}

void MutablePriorityQueue::insert(Vertex *x) {
    H.push_back(x);
    heapifyUp(H.size()-1);
}

void MutablePriorityQueue::heapifyUp(unsigned i) {
    Vertex* x = H[i];
    while (i > 1 && x->queueValue < H[parent(i)]->queueValue) {
        set(i, H[parent(i)]);
        i = parent(i);
    }
    set(i, x);
}

void MutablePriorityQueue::decreaseKey(Vertex *x) {
    heapifyUp(x->queueIndex);
}

void MutablePriorityQueue::heapifyDown(unsigned i) {
    Vertex* x = H[i];
    while (true) {
        unsigned k = leftChild(i);
        if (k >= H.size())
            break;
        if (k+1 < H.size() && H[k+1]->queueValue < H[k]->queueValue)
            k++; // right child of i
        if (H[k]->queueValue >= x->queueValue)
            break;
        set(i, H[k]);
        i = k;
    }
    set(i, x);
}

void MutablePriorityQueue::set(unsigned i, Vertex *x) {
    H[i] = x;
    x->queueIndex = i;
}