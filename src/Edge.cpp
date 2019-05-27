#include "Edge.h"

int Edge::edgeCounter = 0;

//Edge::Edge(Vertex *d, Vertex *s, double w): dest(d), weight(w) , src(s){}

Edge::Edge(Vertex *d, double w): dest(d), weight(w) {
    edgeCounter++;
    id = edgeCounter;
}

Vertex* Edge::getDest() {
    return this->dest;
}

int Edge::getId() const {
    return id;
}
