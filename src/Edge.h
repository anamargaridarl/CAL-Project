#ifndef CAL_PROJECT_EDGE_H
#define CAL_PROJECT_EDGE_H

class Vertex;

class Edge {
    static int edgeCounter;
    Vertex * dest;      // destination vertex
    double weight;         // edge weight
    int id;
public:
    Edge(Vertex *d, double w);
    //Edge(Vertex *d, Vertex* s, double w);
    friend class Graph;
    friend class Vertex;
    Vertex* getDest();

    int getId() const;
};

#endif //CAL_PROJECT_EDGE_H
