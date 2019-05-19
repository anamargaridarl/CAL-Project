#include "Edge.h"

Edge::Edge(Vertex *d, Vertex *s, double w): dest(d), weight(w) , src(s){}

Edge::Edge(Vertex *d, double w): dest(d), weight(w) {}

Vertex* Edge::getDest() {
    return this->dest;
}
