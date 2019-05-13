#include <algorithm>

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
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

template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
    vector<T> res;
    Vertex<T> *d = findVertex(dest);
    while(d->path != NULL) {
        res.push_back(d->getInfo());
        d = d->path;
    }
    res.push_back(origin);
    reverse(res.begin(),res.end());
    return res;
}
