#include <algorithm>
#include "Graph.h"

void Graph::dijkstraShortestPath(const nodeInfo &origin, const nodeInfo &end) {
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
                w.dest->queueValue = v->getDist() + w.weight;
                w.dest->path = v;
                if (oldDist == INF)
                    q.insert(w.dest);
                else
                    q.decreaseKey(w.dest);
            }
        }
    }
}