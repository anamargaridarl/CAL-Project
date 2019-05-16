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

    if(!nodesFile.is_open() || !edgesFile.is_open())
    {
        cout << "The path: " << nodesFilePath << endl << "Is INVALID!" << endl;
        return graph;
    }

    loadNodes(graph, nodesFile);
    loadEdges(graph, edgesFile);

    return graph;
}

void loadNodes(Graph<nodeInfo> &graph, ifstream &nodesFile) {
    cout << "Loading nodes..." << endl;
    string line;
    getline(nodesFile, line);

    int numNodes;
    try{
        numNodes = stoi(line);
    }
    catch (invalid_argument)
    {
        cout << "Invalid Format in Nodes file!" << endl;
        return;
    }

    for (unsigned long int i = 0; i < numNodes; i++) {
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
    cout << "Loading edges..." << endl;
    string line;
    getline(edgesFile, line);
    int numEdges;
    try{
        numEdges = stoi(line);
    }
    catch (invalid_argument)
    {
        cout << "Invalid Format in Edges file!" << endl;
        return;
    }

    for (unsigned long int i = 0; i < numEdges; i++) {
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
