#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"
#include "Option.h"
#include "Vehicle.h"
#include "Menu.h"
#include "GraphImporter.h"
#include "GraphViewer/graphviewer.h"
#include "Dijkstra.tpp"
#include <dirent.h>

using namespace std;

GraphViewer *gv = new GraphViewer(600, 600, false);

vector<Vehicle*> vehicles;
Graph<nodeInfo> graph;
//TEMPORARY TO TEST DIJKSTRA
Vertex<nodeInfo>* startVertex;
Vertex<nodeInfo>* endVertex;

void displayMap()
{
    if(graph.getVertexSet().empty())
    {
        cout << "There's no map Loaded!" << endl;
        return;
    }

    Vertex<nodeInfo> *firstVertex = graph.getVertexSet()[0];
    int offsetX = firstVertex->getInfo().lat;
    int offsetY = firstVertex->getInfo().lon;

    for (Vertex<nodeInfo> *v : graph.getVertexSet()) {
        gv->addNode(v->getInfo().nodeID, v->getInfo().lat-offsetX, v->getInfo().lon-offsetY);
    }

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
        }
    }
    gv->rearrange();
}

void displayPath(nodeInfo start, nodeInfo end, vector<nodeInfo> path)
{
    displayMap();
    for (int i = 1; i < path.size() - 1; i++) {
        gv->setVertexColor(path[i].nodeID, "yellow");
    }
    gv->setVertexColor(start.nodeID, "green");
    gv->setVertexColor(end.nodeID, "red");
    gv->rearrange();
}

void loadChosenMap(string name)
{
    graph = importGraph("../GraphFiles/" + name + "/T08_nodes_X_Y_" + name + ".txt", "../GraphFiles/" + name + "/T08_edges_" + name + ".txt", "");
}

void loadMapMenu()
{
    vector<Option*> options;

    DIR *dir;
    if ((dir = opendir ("../GraphFiles/")) != NULL) {
        struct dirent *currentFolder;
        while ((currentFolder = readdir (dir)) != NULL) {
            if((strcmp(currentFolder->d_name, ".") == 0) || (strcmp(currentFolder->d_name, "..") == 0))continue;
            else options.push_back(new StringOption(currentFolder->d_name, loadChosenMap));
        }
        closedir (dir);
    } else {
        perror ("GraphFiles Directory not found!");
        return;
    }
    options.push_back(new Option("View Loaded Map", displayMap));
    Menu loadMapMenu("Choose Map to Load", options);

    loadMapMenu.run();

    /*
    //TEMP
    vector<nodeInfo> nodes;

    nodeInfo n1(90379619);
    nodeInfo n2(90379359);
    nodeInfo n3(122586155);

    nodes.push_back(n1);
    nodes.push_back(n2);
    nodes.push_back(n3);

    graph.clarkeWright(nodes);
    */
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
    vector<Option*> options;
    options.push_back(new Option("List all Vehicles", listAllVehicles));
    options.push_back(new Option("Create New Vehicle", vehicleCreation));
    options.push_back(new Option("Remove Vehicle", removeVehicle));
    Menu vehiclesMenu("Vehicles Menu", options);

    vehiclesMenu.run();
}

void createJourneyMenu()
{
    if(graph.getVertexSet().empty())
    {
        cout << "There is no map currently loaded!\n    Please Load a Map!" << endl;
        return;
    }
    int startPointID = -1;
    int finalPointID = -1;

    cout << "Insert ID of the start point for all Vehicles: " << flush;
    while(!(cin >> startPointID) || (startVertex = graph.findVertex(nodeInfo(startPointID))) == NULL)
    {
        cout << "ERROR: Invalid Location!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Insert the start point for all Vehicles: " << flush;
    }

    cout << "TEMPORARY: Insert ID of the destination: " << flush;
    while(!(cin >> finalPointID) || (endVertex = graph.findVertex(nodeInfo(finalPointID))) == NULL)
    {
        cout << "ERROR: Invalid Location!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Insert ID of the destination: " << flush;
    }

    //Show the 2 points on the Map
    cout << "START ID: " << startVertex->getInfo().nodeID << "   END ID: " << endVertex->getInfo().nodeID << endl;

    graph.dijkstraShortestPath(startVertex->getInfo(), endVertex->getInfo());

    vector<nodeInfo> path = graph.getPath(startVertex->getInfo(), endVertex->getInfo());

    displayPath(startVertex->getInfo(), endVertex->getInfo(), path);

    /* USAR ISTO SÓ QND TIVERMOS Clarke e Wreight
    while(true)
    {
        string userIntput;
        Vertex<nodeInfo>* retrievalVertex;
        bool invalidID = true;
        //tuple<RetrievalVertex, List<DeliveryVertex>>
        do
        {
            int retrievalID = -1;
            cout << "Insert the ID of a point of retrieval(! to cancel): " << flush;
            cin >> userIntput;
            if (userIntput == "!") return;
            try{
                retrievalID = stoi(userIntput);
            }
            catch(invalid_argument)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Input!" << endl;
                continue;
            }

            invalidID = ((retrievalVertex = graph.findVertex(retrievalID)) == NULL);
            if(invalidID)
            {
                cout << "ERROR: Invalid Location!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }while(invalidID);

        invalidID = true;
        Vertex<nodeInfo>* deliveryVertex;
        bool insertingDelivery = true;
        while(insertingDelivery) {
            do {
                int deliveryID = -1;
                cout << "Insert the ID of a point of delivery for the previous retrieval(! to cancel): " << flush;
                cin >> userIntput;
                if (userIntput == "!")
                {
                    insertingDelivery = false;
                    break;
                }
                try {
                    deliveryID = stoi(userIntput);
                }
                catch (invalid_argument) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid Input!" << endl;
                    continue;
                }

                invalidID = ((deliveryVertex = graph.findVertex(deliveryID)) == NULL ||
                             deliveryVertex == retrievalVertex);
                {
                    cout << "ERROR: Invalid Location!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            } while (invalidID);
        }
    }
     */
}

void mainMenu()
{
    vector<Option*> options;
    options.push_back(new Option("Load Map", loadMapMenu));
    options.push_back(new Option("Vehicles", vehiclesMenu));
    options.push_back(new Option("Create Journey", createJourneyMenu));
    Menu mainMenu("Main Menu", options);

    mainMenu.run();
}

int main() {
    gv->createWindow(600, 600);
    gv->defineVertexColor("blue");
    gv->defineEdgeColor("black");
    mainMenu();
    return 0;
}