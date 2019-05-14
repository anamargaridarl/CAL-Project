#include <algorithm>

template<class T>
void Graph<T>::aStarShortestPath(const T &origin, const T &end) {
    for(auto v: vertexSet)
    {
        v->dist = INF;
        v->path = NULL;
    }

    Vertex<T> *s = findVertex(origin);
    s->dist = 0;
    s->path = NULL;

    MutablePriorityQueue<Vertex<T>> q;

    q.insert(s);

    while(!q.empty())
    {
        Vertex<T> *v = q.extractMin();
        if(v->getInfo() == end)
            break;
        for(auto w: v->adj)
        {
            double oldDist = w.dest->getDist();
            if(w.dest->getDist() > v->getDist() + w.weight) {
                w.dest->dist = v->getDist() + w.weight;
                w.dest->path = v;
                if (oldDist == INF)
                    q.insert(w.dest+cartesianDistance(&v->getInfo(), &end));
                else
                    q.decreaseKey(w.dest+cartesianDistance(&v->getInfo(), &end));
            }
        }
    }
}

template<class T>
double cartesianDistance (T* point1, const T* point2) {
    nodeInfo* info1 = (nodeInfo*) point1;
    nodeInfo* info2 = (nodeInfo*) point2;
    return (sqrt(pow(info1->lon-info2->lon,2)+pow(info1->lat-info2->lat,2)));
}