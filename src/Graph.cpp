#include "Graph.h"

int Graph::getNumVertex() const {
    return vertexSet.size();
}

vector<Vertex*> Graph::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */

Vertex* Graph::findVertex(const nodeInfo &in) const {
    for (auto v : vertexSet)
        if (v->info == in)
            return v;
    return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
bool Graph::addVertex(const nodeInfo &in) {
    if ( findVertex(in) != NULL)
        return false;
    vertexSet.push_back(new Vertex(in));
    return true;
}

/*
 * Clears the Graph's information
 */
void Graph::clear() {
    for(Vertex* vertex : vertexSet)
    {
        delete vertex;
    }

    vertexSet.clear();
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
bool Graph::addEdge(const nodeInfo &sourc, const nodeInfo &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL)
        return false;
    v1->addEdge(v2,w);
    return true;
}