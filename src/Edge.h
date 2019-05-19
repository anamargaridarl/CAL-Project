#ifndef CAL_PROJECT_EDGE_H
#define CAL_PROJECT_EDGE_H

class Vertex;

class Edge {
    Vertex * dest;      // destination vertex
    double weight;         // edge weight
public:
    Edge(Vertex *d, double w);
    //Edge(Vertex *d, Vertex* s, double w);
    friend class Graph;
    friend class Vertex;
    Vertex* getDest();
};

#endif //CAL_PROJECT_EDGE_H
