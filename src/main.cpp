#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"
#include "Option.h"
#include "Vehicle.h"
#include "Menu.h"
#include "GraphImporter.h"
#include "GraphViewer/graphviewer.h"

using namespace std;

vector<Vehicle*> vehicles;
Graph<nodeInfo> graph;

void loadMapMenu()
{
    graph = importGraph("../GraphFiles/Porto/T08_nodes_X_Y_Porto.txt", "../GraphFiles/Porto/T08_edges_Porto.txt", "");
}

void vehicleCreation()
{
    unsigned int capacity;
    cout << "----Vehicle Creation----" << endl;

    cout << "-Insert Vehicle Capacity: ";
    while(!(cin >> capacity))
    {
        cout << "Invalid Capacity!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Insert Vehicle Capacity: ";
    }
    cout << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Vehicle* v = new Vehicle(capacity, Love);

    vehicles.push_back(v);
}

void listAllVehicles()
{
    cout << "------------------------" << endl;
    for(unsigned int i = 0; i < vehicles.size(); i++)
    {
        cout << i << ": " << vehicles.at(i)->getInfo() << endl;
    }
    cout << "------------------------" << endl;
}

void removeVehicle()
{
    listAllVehicles();

    string answer;
    int pos = -1;

    do {
        cout << "Insert the Vehicle to remove(! to cancel): " << flush;
        cin >> answer;
        if (answer == "!") return;
        try{
            pos = stoi(answer);
        }
        catch(invalid_argument)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            cout << "Invalid Answer!" << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.clear();
        if(pos < 0 || pos >= vehicles.size()) cout << "Invalid Answer!" << endl;
    }while(pos < 0 || pos >= vehicles.size());

    Vehicle* vehicle = vehicles.at(pos);
    vehicles.erase(vehicles.begin() + pos);
    delete vehicle;
}

void vehiclesMenu()
{
    vector<Option> options;
    options.push_back(Option("List all Vehicles", listAllVehicles));
    options.push_back(Option("Create New Vehicle", vehicleCreation));
    options.push_back(Option("Remove Vehicle", removeVehicle));
    Menu vehiclesMenu("Vehicles Menu", options);

    vehiclesMenu.run();
}

void createJourneyMenu()
{
    cout << "----!WIP!----" << endl << endl;
}

void displayMap()
{
    GraphViewer *gv = new GraphViewer(600, 600, false);

    gv->createWindow(600, 600);
    gv->defineVertexColor("blue");
    gv->defineEdgeColor("black");

    for (Vertex<nodeInfo> *v : graph.getVertexSet()) {
        gv->addNode(v->getInfo().nodeID, v->getInfo().lat-527509, v->getInfo().lon-4556047);
    }

    gv->addNode(0, 0, 0);

    int i = 0;
    for (Vertex<nodeInfo> *v : graph.getVertexSet()) {
        for (Edge<nodeInfo> e : v->getEdges()) {
            bool bidirect = false;
            for(Edge<nodeInfo> e2 : e.getDest()->getEdges()) {
                if(e2.getDest() == v) {
                    bidirect = true;
                    break;
                }
            }
            if(bidirect)
                gv->addEdge(i++,v->getInfo().nodeID, e.getDest()->getInfo().nodeID, EdgeType::UNDIRECTED);
            else
                gv->addEdge(i++,v->getInfo().nodeID, e.getDest()->getInfo().nodeID, EdgeType::DIRECTED);
            cout << i << endl;
        }
    }

    //TEMPORARIO
    nodeInfo start;
    start.nodeID = 90379619;
    nodeInfo end;
    end.nodeID = 1108123577;
    graph.dijkstraShortestPath(start, end);
    vector<nodeInfo> path = graph.getPath(start,end);
    gv->setVertexColor(start.nodeID, "red");
    gv->setVertexColor(end.nodeID, "green");
    for (int i = 1; i < path.size() - 1; i++) {
        gv->setVertexColor(path[i].nodeID, "yellow");
    }
    gv->rearrange();
    //TEMPORARIO END
}

void mainMenu()
{
    vector<Option> options;
    options.push_back(Option("Load Map", loadMapMenu));
    options.push_back(Option("Vehicles", vehiclesMenu));
    options.push_back(Option("Create Journey", createJourneyMenu));
    options.push_back(Option("Display Map", displayMap));
    Menu mainMenu("Main Menu", options);

    mainMenu.run();
}

int main() {
    mainMenu();
    return 0;
}