#include "Vertex.h"

Vertex::Vertex(nodeInfo in): info(in) {}

vector<Edge> Vertex::getEdges() {
    return adj;
}

void Vertex::addEdge(Vertex *d, double w) {
    adj.push_back(Edge(d, w));
}

bool Vertex::operator<(Vertex & vertex) const {
    return this->dist < vertex.dist;
}

nodeInfo Vertex::getInfo() const {
    return this->info;
}

double Vertex::getDist() const {
    return this->dist;
}

Vertex *Vertex::getPath() const {
    return this->path;
}

bool Vertex::isVisited() {
    return visited;
}
