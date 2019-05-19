#include <algorithm>
#include "Graph.h"

double cartesianDistance (nodeInfo* point1, const nodeInfo* point2) {
    return (sqrt(pow(point1->lon-point2->lon,2)+pow(point1->lat-point2->lat,2)));
}

void Graph::aStarShortestPath(const nodeInfo &origin, const nodeInfo &end) {
    for(auto v: vertexSet)
    {
        v->dist = INF;
        v->path = NULL;
    }

    Vertex *s = findVertex(origin);
    s->dist = 0;
    s->path = NULL;

    MutablePriorityQueue<Vertex> q;

    q.insert(s);

    while(!q.empty())
    {
        Vertex *v = q.extractMin();
        if(v->getInfo() == end)
            break;
        for(auto w: v->adj)
        {
            double oldDist = w.dest->getDist();
            if(w.dest->getDist() > v->getDist() + w.weight) {
                w.dest->dist = v->getDist() + w.weight;
                w.dest->path = v;
                if (oldDist == INF)
                    q.insert(w.dest);
                else
                    q.decreaseKey(w.dest);
            }
        }
    }
}