/*
 * MutablePriorityQueue.h
 * A simple implementation of mutable priority queues, required by Dijkstra algorithm.
 *
 * Created on: 17/03/2018
 *      Author: Joao Pascoal Faria
 */

#ifndef SRC_MUTABLEPRIORITYQUEUE_H_
#define SRC_MUTABLEPRIORITYQUEUE_H_

#include <vector>
#include "Vertex.h"


using namespace std;

/**
 * class T must have: (i) accessible field int queueIndex; (ii) operator< defined.
 */

class MutablePriorityQueue {
	vector<Vertex *> H;
	void heapifyUp(unsigned i);
	void heapifyDown(unsigned i);
	inline void set(unsigned i, Vertex * x);
public:
	MutablePriorityQueue();
	void insert(Vertex * x);
	Vertex * extractMin();
	void decreaseKey(Vertex * x);
	bool empty();
};

// Index calculations
#define parent(i) ((i) >> 1)  /* i / 2 */
#define leftChild(i) ((i) << 1)  /* i * 2 */

#endif /* SRC_MUTABLEPRIORITYQUEUE_H_ */
