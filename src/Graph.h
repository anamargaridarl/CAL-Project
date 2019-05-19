/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"
#include "Edge.h"

using namespace std;

#define INF numeric_limits<double>::max();

/*************************** Graph  **************************/

class Graph {
	vector<Vertex *> vertexSet;    // vertex set

public:
	Vertex *findVertex(const nodeInfo &in) const;
	bool addVertex(const nodeInfo &in);
	bool addEdge(const nodeInfo &sourc, const nodeInfo &dest, double w);
	int getNumVertex() const;
	vector<Vertex *> getVertexSet() const;


	void dijkstraShortestPath(const nodeInfo &s, const nodeInfo &e);
    void aStarShortestPath(const nodeInfo &s, const nodeInfo &e);
    vector<vector<nodeInfo>> nearestNeighbour(nodeInfo startPoint, vector<nodeInfo> points);

	vector<nodeInfo> getPath(const nodeInfo &origin, const nodeInfo &dest) const;
    int getCost(const nodeInfo &dest) const;
    void clear();
};

/**************** Single Source Shortest Path algorithms ************/



#endif /* GRAPH_H_ */
