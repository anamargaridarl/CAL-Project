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
vector<nodeInfo> currentPath;

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
            gv->addEdge(e.getId(),v->getInfo().nodeID, e.getDest()->getInfo().nodeID, EdgeType::DIRECTED);
        }
    }

    graphViewerLoaded = true;
    gv->rearrange();
}

void clearPreviousPath() {
    for (int i = 0; i < currentPath.size(); i++) {
        gv->setVertexColor(currentPath[i].nodeID, "blue");
        vector<Edge> edges = graph.findVertex(currentPath[i].nodeID)->getEdges();
        for(Edge edge : edges)
        {
            gv->setEdgeColor(edge.getId(), "black");
        }
    }
}

void showPoint(int pointID, string color)
{
    gv->setVertexColor(pointID, color);
}

void displayPath(nodeInfo start, vector<nodeInfo> retrievalPoints, vector<nodeInfo> deliveries, vector<nodeInfo> path)
{
    cout << "START POINT: " << start.nodeID << endl;
    currentPath = path;
    for (int i = 1; i < path.size(); i++) {
        gv->setVertexColor(path[i].nodeID, "yellow");
        gv->setEdgeColor(graph.findVertex(path[i-1].nodeID)->getEdge(graph.findVertex(path[i].nodeID)).getId(), "yellow");
        cout << "Next: " << path[i].nodeID << endl;
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
        vector<Edge> edges = v->getEdges();
        for(Edge edge : edges)
        {
            gv->setEdgeColor(edge.getId(), "black");
        }
    }
}

void loadChosenMap(string name)
{
    cout << "Would you like to load the map as Bidirectional?(Y or N)" << flush;
    string answer = "";
    bool bidirectional = false;
    while(!(cin >> answer) || (answer != "Y" && answer != "N"))
    {
        cout << "Invalid Answer (Enter Y or N): " << flush;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    bidirectional = (answer == "Y");

    if(graphViewerLoaded) clearGraphViewer();
    graph.clear();
    graph = importGraph("../GraphFiles/" + name + "/T08_nodes_X_Y_" + name + ".txt", "../GraphFiles/" + name + "/T08_edges_" + name + ".txt", "", bidirectional);
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
    Menu loadMapMenu("Choose Map to Load", options);

    loadMapMenu.run();
}

void vehicleCreation()
{
    unsigned int specialization;
    cout << "----Vehicle Creation----" << endl;

    cout << "Insert Vehicle Specialization: " << endl;
    cout << "0: Money " << endl;
    cout << "1: Art " << endl;
    cout << "2: Love " << endl;

    while(!(cin >> specialization) || specialization > 2 || specialization < 0)
    {
        cout << "Invalid Specialization!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Insert Vehicle Specialization: ";
        cout << "1.Money " << endl;
        cout << "2.Art " << endl;
        cout << "3.Love " << endl;
    }
    cout << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    Vehicle* v = new Vehicle(specialization);



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
    if (vehicles.empty()) {
        cout << "There are no Vehicles in your fleet!\n    Please Create your vehicles first!" << endl;
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
            catch (exception &e) {
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

    showPoint(startPointID, "green");

    nodeInfo startPoint(startPointID);
    Vertex *startVertex = graph.findVertex(startPoint);
    vertexList.push_back(startVertex);

    vector < tuple < nodeInfo, vector < nodeInfo >> > deliveries[3];
    vector <nodeInfo> allDeliveryPoints;
    vector <nodeInfo> allRetrievalPoints;
    while (true) {
        int retrievalID = -1;
        cout << "Insert the ID of a point of retrieval(! to cancel): " << flush;
        if (!readPointID(retrievalID)) break;
        showPoint(retrievalID, "red");

        int specialization = -1;
        cout << "Insert the type of Merch: " << endl;
        cout << "0: Money " << endl;
        cout << "1: Art " << endl;
        cout << "2: Love " << endl;

        while(!(cin >> specialization) || specialization > 2|| specialization < 0)
        {
            cout << "Invalid Merch Type!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Insert the type of Merch: " << endl;
            cout << "0: Money " << endl;
            cout << "1: Art " << endl;
            cout << "2: Love " << endl;
        }
        cout << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        nodeInfo retrievalPoint(retrievalID);
        vertexList.push_back(graph.findVertex(retrievalPoint));

        vector <nodeInfo> deliveryPoints;
        while (true) {
            int deliveryID = -1;
            int quantity = 0;
            cout << "Insert the ID of a point of delivery for the previous retrieval(! to cancel): " << flush;
            if (!readPointID(deliveryID)) break;
            showPoint(deliveryID, "magenta");
            cout << "Insert quantity of goods" << endl;
            cin >> quantity;
            nodeInfo deliveryPoint(deliveryID);
            deliveryPoint.setQuantity(quantity);
            deliveryPoints.push_back(deliveryPoint);
            vertexList.push_back(graph.findVertex(deliveryPoint));
        }

        if(deliveryPoints.empty())
        {
            cout << "Retrieval point had no deliveries so it was ignored!" << endl;
            continue;
        }

        allDeliveryPoints.insert(allDeliveryPoints.begin(), deliveryPoints.begin(), deliveryPoints.end());
        allRetrievalPoints.push_back(retrievalPoint);
        tuple <nodeInfo, vector<nodeInfo>> delivery = make_tuple(retrievalPoint, deliveryPoints);
        deliveries[specialization].push_back(delivery);
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

    vector<Vehicle*> moneyVehicles;
    vector<Vehicle*> artVehicles;
    vector<Vehicle*> loveVehicles;

    for(Vehicle* vehicle : vehicles)
    {
        switch(vehicle->getTypeOfMerch())
        {
            case Money:
                moneyVehicles.push_back(vehicle);
                break;
            case Art:
                artVehicles.push_back(vehicle);
                break;
            case Love:
                loveVehicles.push_back(vehicle);
                break;
        }
    }

    vector < pair < Vehicle * , vector < tuple < nodeInfo,
            vector < nodeInfo >> >> > moneyPaths;
    vector < pair < Vehicle * , vector < tuple < nodeInfo,
            vector < nodeInfo >> >> > artPaths;
    vector < pair < Vehicle * , vector < tuple < nodeInfo,
            vector < nodeInfo >> >> > lovePaths;

    if(!moneyVehicles.empty())
        moneyPaths = divideVehicles(moneyVehicles, deliveries[0]);


    if(!artVehicles.empty())
     artPaths = divideVehicles(artVehicles, deliveries[1]);

    if(!loveVehicles.empty())
        lovePaths = divideVehicles(loveVehicles, deliveries[2]);

    vector < pair < Vehicle * , vector < tuple < nodeInfo,
            vector < nodeInfo >> >> > paths[3] = {moneyPaths, artPaths, lovePaths};

    int vehicleDisplay = -1;
    int flagDisplay = 1;

    while (flagDisplay) {
        cout << "\nChoose an option: " << endl;
        cout << "0: Exit" << endl;

        int y = 1;
        for (int i = 0; i < 3; i++) {
            for(int j = 1; j <= paths[i].size(); j++)
            {
                cout << y << ": Display Path of " << (paths[i][j-1].first)->getType() << " Vehicle nº " << j << endl;
                y++;
            }
        }

        while(!(cin >> vehicleDisplay) || vehicleDisplay < 0 || vehicleDisplay > y)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid Option! Insert a valid number: " << flush;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(vehicleDisplay == 0)
            break;

        int x = vehicleDisplay;
        int row = 0;
        int choice;
        for(int i = 0; i < 3; i++)
        {
            choice = x - 1;
            if((x -= paths[i].size()) <= 0)
            {
                row = i;
                break;
            }
        }

        vehicleDisplay--;

        vector <nodeInfo> del; //deliveries
        vector <nodeInfo> ret; //retrivals

        del.clear();
        ret.clear();

        for (tuple <nodeInfo, vector<nodeInfo>> request: paths[row].at(choice).second) {
            for (nodeInfo n: get<1>(request)) {
                del.push_back(n);
            }
            ret.push_back(get<0>(request));
        }

        vector <nodeInfo> path = graph.nearestNeighbour(startPoint, paths[row].at(choice).second, false, true);
        if(path.empty())
        {
            cout << "Impossible to create a Route" << endl;
            return;
        }
        clearPreviousPath();
        displayPath(startPoint, ret, del, path);

    }

}

void testAlgorithms() {
    graph = importGraph("../GraphFiles/Porto/T08_nodes_X_Y_Porto.txt", "../GraphFiles/Porto/T08_edges_Porto.txt", "", true);
    clock_t dfs_start = clock();

    graph.dfs(graph.findVertex(nodeInfo(1108123561)));
    clock_t dfs_finish = clock();
    cout << "DFS: " << dfs_finish - dfs_start << "ms" << endl;

    clock_t dijkstra1_start = clock();
    graph.dijkstraShortestPath(nodeInfo(312403909), nodeInfo(312404122));
    clock_t dijkstra1_finish = clock();
    cout << "Dijkstra (small distance): " << graph.findVertex(nodeInfo(312404122))->getDist() << " cost, " << dijkstra1_finish - dijkstra1_start << "ms" << endl;

    clock_t astar1_start = clock();
    graph.aStarShortestPath(nodeInfo(312403909), nodeInfo(312404122));
    clock_t astar1_finish = clock();
    cout << "A-star (small distance): " << graph.findVertex(nodeInfo(312404122))->getDist() << " cost, " << astar1_finish - astar1_start << "ms" << endl;

    clock_t dijkstra2_start = clock();
    graph.dijkstraShortestPath(nodeInfo(312403909), nodeInfo(495503510));
    clock_t dijkstra2_finish = clock();
    cout << "Dijkstra (medium distance): " << graph.findVertex(nodeInfo(495503510))->getDist() << " cost, " << dijkstra2_finish - dijkstra2_start << "ms" << endl;

    clock_t astar2_start = clock();
    graph.aStarShortestPath(nodeInfo(312403909), nodeInfo(495503510));
    clock_t astar2_finish = clock();
    cout << "A-star (medium distance): " << graph.findVertex(nodeInfo(495503510))->getDist() << " cost, " << astar2_finish - astar2_start << "ms" << endl;

    clock_t dijkstra3_start = clock();
    graph.dijkstraShortestPath(nodeInfo(312403909), nodeInfo(299611722));
    clock_t dijkstra3_finish = clock();
    cout << "Dijkstra (large distance): " << graph.findVertex(nodeInfo(299611722))->getDist() << " cost, " << dijkstra3_finish - dijkstra3_start << "ms" << endl;

    clock_t astar3_start = clock();
    graph.aStarShortestPath(nodeInfo(312403909), nodeInfo(299611722));
    clock_t astar3_finish = clock();
    cout << "A-star (large distance): " << graph.findVertex(nodeInfo(299611722))->getDist() << " cost, " << astar3_finish - astar3_start << "ms" << endl;

    nodeInfo startPoint(311887368);

    nodeInfo retriaval1(314075202);
    nodeInfo rd1(311887395);
    nodeInfo rd2(314075673);
    vector<nodeInfo> r1; r1.push_back(rd1); r1.push_back(rd2);
    tuple<nodeInfo, vector<nodeInfo>> delivery1 = make_tuple(retriaval1, r1);

    nodeInfo retriaval2(314075690);
    nodeInfo rd3(1108123763);
    vector<nodeInfo> r2; r2.push_back(rd3);
    tuple<nodeInfo, vector<nodeInfo>> delivery2 = make_tuple(retriaval2, r2);
    vector<tuple<nodeInfo, vector<nodeInfo>>> poi; poi.push_back(delivery1); poi.push_back(delivery2);

    clock_t nn_start = clock();
    vector<nodeInfo> path = graph.nearestNeighbour(startPoint, poi, false, true);
    if(path.empty())
    {
        cout << "Impossible to create a Route" << endl;
        return;
    }
    clock_t nn_finish = clock();
    cout << "Nearest Neighbour: " << graph.getPathCost(path) << " cost, " << nn_finish - nn_start << "ms" << endl;

    clock_t opt_start = clock();
    vector<nodeInfo> path1 = graph.nearestNeighbour(startPoint, poi, true, true);
    clock_t opt_finish = clock();
    cout << "Nearest Neighbour with 2Opt: " << graph.getPathCost(path1) << " cost, " << opt_finish - opt_start << "ms" << endl;
}

void mainMenu()
{
    vector<Option*> options;
    options.push_back(new Option("Load Map", loadMapMenu));
    options.push_back(new Option("Vehicles", vehiclesMenu));
    options.push_back(new Option("Create Journey", createJourneyMenu));
    options.push_back(new Option("Test Algorithms", testAlgorithms));
    Menu mainMenu("Main Menu", options);

    mainMenu.run();
}

int main() {
    gv->createWindow(600, 600);
    gv->defineVertexColor("blue");
    gv->defineEdgeColor("black");
    bool quit = false;
    do{
        mainMenu();
        cout << "Are you sure you want to quit? (Y or N): " << flush;
        string answer = "";
        while(!(cin >> answer) || (answer != "Y" && answer != "N"))
        {
            cout << "Invalid Answer (Enter Y or N): " << flush;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if(answer == "Y") quit = true;
    } while (!quit);

    return 0;
}