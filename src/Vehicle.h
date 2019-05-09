#ifndef PROJECT_VEHICLE_H
#define PROJECT_VEHICLE_H

enum merchType {
    Money,
    Art,
    Love //<3
};

class Vehicle {
public:
    Vehicle(unsigned int capacity, merchType typeOfMerch);

private:
    unsigned int capacity;
    merchType typeOfMerch;
};


#endif //PROJECT_VEHICLE_H
