#include <algorithm>
#include <iostream>
#include "Graph.h"

void Graph::aStarShortestPath(const nodeInfo &origin, const nodeInfo &end) {
    for(auto v: vertexSet)
    {
        v->dist = INF;
        v->path = NULL;
        v->queueValue = 0;
        v->queueIndex = 0;
    }

    Vertex *s = findVertex(origin);
    s->dist = 0;
    s->path = NULL;
    Vertex *e = findVertex(end);

    MutablePriorityQueue q;

    q.insert(s);

    Vertex *v;
    while(!q.empty() || v->info == end)
    {
        v = q.extractMin();
        if(v->getInfo() == end)
            break;
        for(auto w: v->adj)
        {
            double oldDist = w.dest->getDist();
            if(w.dest->getDist() > v->getDist() + w.weight) {
                w.dest->dist = v->getDist() + w.weight;
                w.dest->queueValue = v->getDist() + w.weight + squaredCartesianDistance(&v->info, &e->info);
                w.dest->path = v;
                if (w.dest->queueIndex == 0) {
                    q.insert(w.dest);
                }
                else {
                    q.decreaseKey(w.dest);
                }
            }
        }
    }
}