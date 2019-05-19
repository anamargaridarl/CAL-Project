#include <algorithm>
#include "Graph.h"

void Graph::dijkstraShortestPath(const nodeInfo &origin, const nodeInfo &end) {
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
                if (oldDist == numeric_limits<double>::max())
                    q.insert(w.dest);
                else
                    q.decreaseKey(w.dest);
            }
        }
    }
}

vector<nodeInfo> Graph::getPath(const nodeInfo &origin, const nodeInfo &dest) const{
    vector<nodeInfo> res;
    Vertex *d = findVertex(dest);
    while(d->path != NULL) {
        res.push_back(d->getInfo());
        d = d->path;
    }
    res.push_back(origin);
    reverse(res.begin(),res.end());
    return res;
}

double Graph::getCost(const nodeInfo &dest) const{
    Vertex *destV = findVertex(dest);
    return destV->getDist();
}