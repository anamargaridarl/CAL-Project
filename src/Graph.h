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

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

typedef struct nodeInfo {
    int nodeID;
    float lat;
    float lon;
    string tag;

    nodeInfo(int ID) : nodeID(ID)
    {
        lat = 0;
        lon = 0;
        tag = "";
    }

    nodeInfo(){}

    inline bool operator==(nodeInfo info) {
        if (info.nodeID==nodeID)
            return true;
        else
            return false;
    }
}nodeInfo;

/************************* Vertex  **************************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // outgoing edges
	bool visited;          // auxiliary field
	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool processing = false;
	void addEdge(Vertex<T> *dest, double w);
public:
	Vertex(T in);
	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	T getInfo() const;
	vector<Edge<T>> getEdges();
	double getDist() const;
	Vertex *getPath() const;
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};

#include "Vertex.tpp"

/********************** Edge  ****************************/
template <class T>
class Edge {
    Vertex<T> * dest;      // destination vertex
    Vertex<T> * src;      // destination vertex
    double weight;         // edge weight
public:
    Edge(Vertex<T> *d, double w);
    Edge(Vertex<T> *d, Vertex<T>* s, double w);
    friend class Graph<T>;
    friend class Vertex<T>;
    Vertex<T>* getDest();
};

#include "Edge.tpp"
#include "GraphViewer/graphviewer.h"

/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set

public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;


	void dijkstraShortestPath(const T &s, const T &e);
    void aStarShortestPath(const T &s, const T &e);
    vector<vector<T>> clarkeWright(vector<T> points);

	vector<T> getPath(const T &origin, const T &dest) const;
    int getCost(const T &dest) const;
    void clear();
};

#include "Graph.tpp"
#include "Dijkstra.tpp"
#include "A-star.tpp"
#include "ClarkeWright.tpp"

/**************** Single Source Shortest Path algorithms ************/



#endif /* GRAPH_H_ */
