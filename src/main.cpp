#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"
#include "Option.h"
#include "Vehicle.h"
#include "Menu.h"
#include "GraphImporter.h"
#include "Dijkstra.tpp"

using namespace std;

vector<Vehicle*> vehicles;
Graph<nodeInfo> graph;
//TEMPORARY TO TEST DIJKSTRA
Vertex<nodeInfo>* startVertex;
Vertex<nodeInfo>* endVertex;

void loadMapMenu()
{
    graph = importGraph("../GraphFiles/Porto/T08_nodes_lat_lon_Porto.txt", "../GraphFiles/Porto/T08_edges_Porto.txt", "");
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
    while(!(cin >> finalPointID) || (endVertex = graph.findVertex(nodeInfo(startPointID))) == NULL)
    {
        cout << "ERROR: Invalid Location!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Insert ID of the destination: " << flush;
    }

    //Show the 2 points on the Map

    graph.getPath(startVertex->getInfo(), endVertex->getInfo());

    //Show trajectory on the Map

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
    vector<Option> options;
    options.push_back(Option("Load Map", loadMapMenu));
    options.push_back(Option("Vehicles", vehiclesMenu));
    options.push_back(Option("Create Journey", createJourneyMenu));
    Menu mainMenu("Main Menu", options);

    mainMenu.run();
}

int main() {
    mainMenu();
    return 0;
}