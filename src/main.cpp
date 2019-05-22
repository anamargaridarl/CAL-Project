#include <iostream>
#include <tuple>
#include <dirent.h>
#include "Graph.h"
#include "GraphImporter.h"
#include "MutablePriorityQueue.h"
#include "Option.h"
#include "Vehicle.h"
#include "Menu.h"
#include "GraphViewer/graphviewer.h"
#include "Dijkstra.cpp"
#include "DivideVehicles.h"

using namespace std;

GraphViewer *gv = new GraphViewer(600, 600, false);
bool graphViewerLoaded = false;

vector<Vehicle*> vehicles;
Graph graph;

long int hashIdPair(long int id1, long int id2) {
    return (long int)(0.5*(id1+id2)*(id1+id2+1)+id2);
}

pair<long int, long int> dehashIdPair(long int hash) {
    double w = floor((sqrt(8 * hash + 1) - 1)/2);
    double t = (w*w+w)/2;
    long int y = (long int)(hash-t);
    long int x = (long int)(w-y);
    return make_pair(x,y);
}

void clearGraphViewer()
{
    int i = 0;
    for (Vertex *v : graph.getVertexSet()) {
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

    Vertex *firstVertex = graph.getVertexSet()[0];
    int offsetX = firstVertex->getInfo().lat;
    int offsetY = firstVertex->getInfo().lon;

    for (Vertex *v : graph.getVertexSet()) {
        gv->addNode(v->getInfo().nodeID, v->getInfo().lat-offsetX, v->getInfo().lon-offsetY);
        for (Edge e : v->getEdges()) {
            gv->addEdge(v->getInfo().nodeID*1000000000+e.getDest()->getInfo().nodeID,v->getInfo().nodeID, e.getDest()->getInfo().nodeID, EdgeType::DIRECTED);
        }
    }

    graphViewerLoaded = true;
    gv->rearrange();
}

void displayPath(nodeInfo start, vector<nodeInfo> retrievalPoints, vector<nodeInfo> deliveries, vector<nodeInfo> path)
{
    displayMap();
    for (int i = 1; i < path.size(); i++) {
        gv->setVertexColor(path[i].nodeID, "yellow");
        gv->setEdgeColor(path[i-1].nodeID*1000000000+path[i].nodeID, "red");
    }
    gv->setVertexColor(start.nodeID, "green");
    for(nodeInfo node : retrievalPoints)
    {
        gv->setVertexColor(node.nodeID, "red");
    }
    for(nodeInfo node : deliveries)
    {
        gv->setVertexColor(node.nodeID, "magenta");
    }
    gv->rearrange();
}

void resetMapPath()
{
    for (Vertex *v : graph.getVertexSet()) {
        gv->setVertexColor(v->getInfo().nodeID, "blue");
    }
}

void loadChosenMap(string name)
{
    if(graphViewerLoaded) clearGraphViewer();
    graph.clear();
    graph = importGraph("../GraphFiles/" + name + "/T08_nodes_X_Y_" + name + ".txt", "../GraphFiles/" + name + "/T08_edges_" + name + ".txt", "");
    displayMap();
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
    //options.push_back(new Option("View Loaded Map", displayMap));
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

void createJourneyMenu() {
    if (graph.getVertexSet().empty()) {
        cout << "There is no map currently loaded!\n    Please Load a Map!" << endl;
        return;
    }
    resetMapPath();

    const auto readPointID = [](int &id) {
        string userInput;
        bool invalidID = true;

        while (invalidID) {
            cin >> userInput;
            if (userInput == "!") return false;
            try {
                id = stoi(userInput);
            }
            catch (invalid_argument &e) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Input!\n Try again: " << flush;
                continue;
            }
            invalidID = (graph.findVertex(id) == nullptr);
            if (invalidID) {
                cout << "ERROR: Invalid Location!\n Try again: " << flush;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        return true;
    };

    vector < Vertex * > vertexList;

    int startPointID = -1;
    cout << "Insert ID of the start point for all Vehicles(! to cancel): " << flush;
    if (!readPointID(startPointID)) {
        cout << "The trip has been Canceled!" << endl;
        return;
    }

    //Show the 2 points on the Map

    nodeInfo startPoint(startPointID);
    Vertex *startVertex = graph.findVertex(startPoint);
    vertexList.push_back(startVertex);

    vector < tuple < nodeInfo, vector < nodeInfo >> > deliveries;
    vector <nodeInfo> allDeliveryPoints;
    vector <nodeInfo> allRetrievalPoints;
    while (true) {
        int retrievalID = -1;
        cout << "Insert the ID of a point of retrieval(! to cancel): " << flush;
        if (!readPointID(retrievalID)) break;
        nodeInfo retrievalPoint(retrievalID);
        vertexList.push_back(graph.findVertex(retrievalPoint));

        vector <nodeInfo> deliveryPoints;
        while (true) {
            int deliveryID = -1;
            int quantity = 0;
            cout << "Insert the ID of a point of delivery for the previous retrieval(! to cancel): " << flush;
            if (!readPointID(deliveryID)) break;
            cout << "Insert quantity of goods" << endl;
            cin >> quantity;
            nodeInfo deliveryPoint(deliveryID);
            deliveryPoint.setQuantity(quantity);
            deliveryPoints.push_back(deliveryPoint);
            vertexList.push_back(graph.findVertex(deliveryPoint));
        }


        //Check if the retrieval has no deliveries (If it doesn't then cancel the retrieval)
        //Check if there is still merch which has not been delivered (When weights are added)

        //Insert the retrieval and it's delivery list into the List of PoI


        allDeliveryPoints.insert(allDeliveryPoints.begin(), deliveryPoints.begin(), deliveryPoints.end());
        allRetrievalPoints.push_back(retrievalPoint);
        tuple <nodeInfo, vector<nodeInfo>> delivery = make_tuple(retrievalPoint, deliveryPoints);
        deliveries.push_back(delivery);
    }

    graph.dfs(startVertex);
    bool possible = true;
    for (auto v : vertexList) {
        if (!v->isVisited()) {
            possible = false;
            cout << "Vertex with ID: " << v->getInfo().nodeID << " Is inaccessible from the starting position" << endl;
        }
    }
    graph.clearVisitedVertexes();
    if (!possible) return;

    vector < pair < Vehicle * , vector < tuple < nodeInfo,
            vector < nodeInfo >> >> > paths = divideVehicles(vehicles, deliveries);

    int vehicleDisplay = 0;
    int flagDisplay = 1;

    while (flagDisplay) {

        for (int i = 1; i <= paths.size(); i++) {

            cout << i << ".Vehicle" << i << endl;

        }

        cout << "0. Exit" << endl;

        cin >> vehicleDisplay;//TODO: handle errors

        if(vehicleDisplay == 0)
            break;

        vehicleDisplay--;

        vector <nodeInfo> del; //deliveries
        vector <nodeInfo> ret; //retrivals

        del.clear();
        ret.clear();

        for (tuple <nodeInfo, vector<nodeInfo>> request: paths.at(vehicleDisplay).second) {
            for (nodeInfo n: get<1>(request)) {
                del.push_back(n);
            }
            ret.push_back(get<0>(request));
        }

        /* testes
        for(pair<Vehicle*, vector<tuple<nodeInfo, vector<nodeInfo>>>> meias: paths) {
            vector < tuple < nodeInfo, vector < nodeInfo >> > batatas = meias.second;
            cout << "veiculo 1 " << endl;
            for (tuple <nodeInfo, vector<nodeInfo>> bananas: batatas) {
                cout << "node recolha" << get<0>(bananas).nodeID << endl;

                vector <nodeInfo> coubes = get<1>(bananas);
                for (nodeInfo cobes:coubes) {
                    cout << "nodes " << cobes.nodeID << endl;
                    cout << "nodes end " << endl;

                }


            }
        }
*/

        vector <nodeInfo> path = graph.nearestNeighbour(startPoint, paths.at(vehicleDisplay).second);
        displayPath(startPoint, ret, del, path);

    }


    /*//TEMPORARY Dijkstra
    graph.dijkstraShortestPath(startPoint, endPoint);
    vector<nodeInfo> path = graph.getPath(startPoint, endPoint);
    displayPath(startPoint, endPoint, path);*/
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