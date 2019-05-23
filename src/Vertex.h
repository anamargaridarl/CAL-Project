#ifndef CAL_PROJECT_VERTEX_H
#define CAL_PROJECT_VERTEX_H

#include <vector>
#include "Edge.h"
#include "MutablePriorityQueue.h"
#include <math.h>

using namespace std;

typedef struct nodeInfo {
    int nodeID;
    float lat;
    float lon;
    int quantity = 0;

    nodeInfo(int ID) : nodeID(ID)
    {
        lat = 0;
        lon = 0;
        quantity = 0;
    }

    nodeInfo(){}

    inline bool operator==(nodeInfo info) {
        if (info.nodeID==nodeID)
            return true;
        else
            return false;
    }

    inline double distance(nodeInfo node)
    {
        return sqrt(pow((lat - node.lat),2) + pow(lon - node.lon, 2));
    }

    void setQuantity(int value)
    {
        quantity = value;
    }

} nodeInfo;

/************************* Vertex  **************************/

class Vertex {
    nodeInfo info;                // contents
    vector<Edge> adj;  // outgoing edges
    bool visited;          // auxiliary field
    double dist = 0;
    Vertex *path = NULL;
    int queueIndex = 0; 		// required by MutablePriorityQueue
    int queueValue = 0;

    bool processing = false;
    void addEdge(Vertex *dest, double w);
public:
    Vertex(nodeInfo in);
    bool operator<(Vertex & vertex) const; // // required by MutablePriorityQueue
    nodeInfo getInfo() const;
    vector<Edge> getEdges();
    double getDist() const;
    Vertex *getPath() const;
    friend class Graph;
    friend class MutablePriorityQueue;
    bool isVisited();
};

#endif //CAL_PROJECT_VERTEX_H
