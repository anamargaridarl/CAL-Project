//
// Created by henri on 10-05-2019.
//

#include "Graph.h"

#ifndef CAL_PROJECT_GRAPHIMPORTER_H
#define CAL_PROJECT_GRAPHIMPORTER_H

#endif //CAL_PROJECT_GRAPHIMPORTER_H

Graph importGraph(string nodesFilePath, string edgesFilePath, string tagsFilePath);

void loadNodes(Graph &graph, ifstream &nodesFile);
void loadEdges(Graph &graph, ifstream &edgesFile);
