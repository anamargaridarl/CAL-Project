#ifndef PROJECT_VEHICLE_H
#define PROJECT_VEHICLE_H

#include <string>
#include <vector>
#include <tuple>
#include "Vertex.h"

enum merchType {
    Money,
    Art,
    Love //<3
};

static std::string merchTypeString[] = {"Money", "Art", "Love"};

class Vehicle {
public:
    Vehicle(unsigned int capacity, merchType typeOfMerch);
    std::string getInfo();
    int getCapacity();
    int setCapacity(int c);
    int getID();
private:
    int id;
    unsigned int capacity;
    merchType typeOfMerch;

};


#endif //PROJECT_VEHICLE_H
