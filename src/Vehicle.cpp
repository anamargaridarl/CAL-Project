#include "Vehicle.h"

using namespace std;

Vehicle::Vehicle(unsigned int capacity, merchType typeOfMerch) : capacity(capacity), typeOfMerch(typeOfMerch) {}

std::string Vehicle::getInfo() {
    return "Capacity: " +  to_string(capacity) + ", Can carry: " + merchTypeString[typeOfMerch];
}

int Vehicle::getCapacity() {
    return capacity;
}

