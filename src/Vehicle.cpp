#include "Vehicle.h"

using namespace std;

int i = 0;

Vehicle::Vehicle(unsigned int capacity, merchType typeOfMerch) : capacity(capacity), typeOfMerch(typeOfMerch) {
    id = i;
    printf("%d\n", id);
    i++;
}

std::string Vehicle::getInfo() {
    return "Capacity: " +  to_string(capacity) + ", Can carry: " + merchTypeString[typeOfMerch];
}

int Vehicle::getCapacity() {
    return capacity;
}

int Vehicle::setCapacity(int c) {
    capacity = c;
}

int Vehicle::getID() {
    return id;
}

