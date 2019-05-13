//
// Created by henri on 10-05-2019.
//
#include <iostream>
#include <fstream>
#include "GraphImporter.h"

using namespace std;

Graph<nodeInfo> importGraph(string nodesFilePath, string edgesFilePath, string tagsFilePath) {
    Graph<nodeInfo> graph;

    ifstream nodesFile(nodesFilePath);
    ifstream edgesFile(edgesFilePath);
    //ifstream tagsFile(tagsFilePath);

    loadNodes(graph, nodesFile);
    loadEdges(graph, edgesFile);

    return graph;
}

void loadNodes(Graph<nodeInfo> &graph, ifstream &nodesFile) {
    string line;
    getline(nodesFile, line);
    int numNodes;
    try{
        numNodes = stoi(line);
    }
    catch (invalid_argument)
    {
        cout << "O Henrique fez merda!" << endl;
        exit(1);
    }

    for (unsigned int i = 0; i < numNodes; i++) {
        int nodeID;
        float lat, lon;

        getline(nodesFile, line);
        sscanf(line.c_str(), "(%d, %f, %f)", &nodeID, &lat, &lon);

        nodeInfo info;
        info.nodeID = nodeID;
        info.lat = lat;
        info.lon = lon;
        info.tag = "";

        graph.addVertex(info);
    }
}

void loadEdges(Graph<nodeInfo> &graph, ifstream &edgesFile) {
    string line;
    getline(edgesFile, line);
    int numEdges = stoi(line);

    for (unsigned int i = 0; i < numEdges; i++) {
        int originNodeID, destNodeID;

        getline(edgesFile, line);
        sscanf(line.c_str(), "(%d, %d)", &originNodeID, &destNodeID);
        nodeInfo origin;
        origin.nodeID = originNodeID;
        nodeInfo dest;
        dest.nodeID = destNodeID;
        graph.addEdge(origin, dest, 1);
    }
}
