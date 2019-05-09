#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"
#include "Option.h"
#include "Vehicle.h"
#include "Menu.h"

using namespace std;

vector<Vehicle*> vehicles;

void loadMapMenu()
{
    cout << "----!WIP!----" << endl << endl;
}

void vehicleCreation()
{
    unsigned int capacity;
    cout << "----Vehicle Creation----" << endl;

    cout << "Insert Vehicle Capacity: ";
    while(!cin >> capacity)
    {
        cout << "Invalid Capacity!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Insert Vehicle Capacity: ";
    }
    cout << endl;

    Vehicle* v = new Vehicle(capacity, Love);

    vehicles.push_back(v);
}

void listAllVehicles()
{

}

void vehiclesMenu()
{
    vector<Option> options;
    options.push_back(Option("List all Vehicles", listAllVehicles));
    options.push_back(Option("Create New Vehicle", vehicleCreation));
    Menu vehiclesMenu("Vehicles Menu", options);

    vehiclesMenu.run();
}

void createJourneyMenu()
{
    cout << "----!WIP!----" << endl << endl;
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

    Vertex<int> A(12);
    cout << A.getPath() << endl;
    cout <<  A.getInfo() << endl;
    return 0;
}