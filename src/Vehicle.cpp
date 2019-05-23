#include "Vehicle.h"

using namespace std;

int i = 0;

Vehicle::Vehicle(unsigned int specialization) {

    if(specialization == 1)
    {
        capacity = 100;
        typeOfMerch = Money;
    }
    else if(specialization == 2)
    {
        capacity = 500;
        typeOfMerch = Art;
    }
    else if(specialization == 3)
    {
        capacity = 40;
        typeOfMerch = Love;
    }

    id = i;
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

