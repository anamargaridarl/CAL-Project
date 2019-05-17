#include <iostream>
#include <tuple>
#include <dirent.h>
#include "Graph.h"
#include "Option.h"
#include "Vehicle.h"
#include "Menu.h"
#include "GraphImporter.h"


using namespace std;

GraphViewer *gv = new GraphViewer(600, 600, false);
bool graphViewerLoaded = false;

vector<Vehicle*> vehicles;
Graph<nodeInfo> graph;

void clearGraphViewer()
{
    int i = 0;
    for (Vertex<nodeInfo> *v : graph.getVertexSet()) {
        gv->removeNode(v->getInfo().nodeID);
        for(int x = 0; x < v->getEdges().size(); x++)
        {
            gv->removeEdge(i);
            i++;
        }
    }
    gv->rearrange();
}

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

    graphViewerLoaded = true;

    int i = 0;
    for (Vertex<nodeInfo> *v : graph.getVertexSet()) {
        gv->addNode(v->getInfo().nodeID, v->getInfo().lat-527509, v->getInfo().lon-4556047);
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
    if(graphViewerLoaded) clearGraphViewer();
    graph.clear();
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

    const auto readPointID = [](int &id) {
        string userInput;
        bool invalidID = true;

        while(invalidID)
        {
            cin >> userInput;
            if (userInput == "!") return false;
            try{
                id = stoi(userInput);
            }
            catch(invalid_argument &e)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Input!" << endl;
                continue;
            }
            invalidID = (graph.findVertex(id) == nullptr);
            if(invalidID)
            {
                cout << "ERROR: Invalid Location!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        return true;
    };

    vector<Vertex<nodeInfo>*> vertexList;

    int startPointID = -1;
    int finalPointID = -1;
    cout << "Insert ID of the start point for all Vehicles(! to cancel): " << flush;
    if(!readPointID(startPointID))
    {
        cout << "The trip has been Canceled!" << endl;
        return;
    }

    cout << "TEMPORARY: Insert ID of the destination(! to cancel): " << flush;
    if(!readPointID(finalPointID))
    {
        cout << "The trip has been Canceled!" << endl;
        return;
    }

    //Show the 2 points on the Map

    nodeInfo startPoint(startPointID);
    nodeInfo endPoint(finalPointID);
    Vertex<nodeInfo>* startVertex = graph.findVertex(startPoint);
    Vertex<nodeInfo>* endVertex = graph.findVertex(endPoint);
    vertexList.push_back(startVertex);
    vertexList.push_back(endVertex);

    ///* USAR ISTO SÓ QND TIVERMOS Clarke e Wreight
    vector<tuple<nodeInfo, vector<nodeInfo>>> deliveries;
    while(true)
    {
        int retrievalID = -1;
        cout << "Insert the ID of a point of retrieval(! to cancel): " << flush;
        if(!readPointID(retrievalID)) break;
        nodeInfo retrievalPoint(retrievalID);

        vector<nodeInfo> deliveryPoints;
        bool insertingDelivery = true;
        while(insertingDelivery) {
            int deliveryID = -1;
            cout << "Insert the ID of a point of delivery for the previous retrieval(! to cancel): " << flush;
            if(!readPointID(deliveryID)) insertingDelivery = false;
            nodeInfo deliveryPoint(deliveryID);
            deliveryPoints.push_back(deliveryPoint);
        }

        //Check if the retrieval has no deliveries (If it doesn't then cancel the retrieval)
        //Check if there is still merch which has not been delivered (When weights are added)

        //Insert the retrieval and it's delivery list into the List of PoI
        tuple<nodeInfo, vector<nodeInfo>> delivery = make_tuple(retrievalPoint, deliveryPoints);
        deliveries.push_back(delivery);
    }
    */

    graph.dfs(startVertex);
    bool possible = true;
    for(auto v : vertexList)
    {
        if(!v->isVisited())
        {
            possible = false;
            cout << "Vertex with ID: " << v->getInfo().nodeID << " Is inaccessible from the starting position" << endl;
        }
    }
    graph.clearVisitedVertexes();
    if(!possible) return;

    //TEMPORARY Dijkstra
    graph.dijkstraShortestPath(startPoint, endPoint);
    vector<nodeInfo> path = graph.getPath(startPoint, endPoint);
    displayPath(startPoint, endPoint, path);
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