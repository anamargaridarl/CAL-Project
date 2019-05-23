//
// Created by anamargaridarl on 20-05-2019.
//
#include "Graph.h"
#include "Vehicle.h"
#include <algorithm>
#include <tuple>
#include <iostream>

using namespace std;

class OverLimitVehicles {
public:
    OverLimitVehicles() {};

    virtual std::string what() const {
        return "Request cant be successfully implemented: Quantity of goods is over the limits of the available vehicles";
    }

};

bool myfunction(int i, int j) { return (i > j); }


vector<pair<Vehicle*, vector<tuple<nodeInfo, vector<nodeInfo>>>>> bestFit(vector<tuple<nodeInfo, vector<nodeInfo>>> deliveries, int n, int c, vector<Vehicle*> v) {


    vector<pair<Vehicle*, vector<tuple<nodeInfo, vector<nodeInfo>>>>>finalPaths;
    vector<int> weights;
    int totalWeight = 0;
    vector<int> weightPerCollectPoint;
    int totalVehicleWeight = 0;
    vector<Vehicle*> aux = v;

    //Calculate weghts for each collectPoint
    for (int i = 0; i < deliveries.size(); i++) {

        vector<nodeInfo> deliveryPoints = get<1>(deliveries.at(i));
        int weight = 0;

        for (int n = 0; n < deliveryPoints.size(); n++) {
            weight += deliveryPoints.at(n).quantity;
        }

        totalWeight+= weight;

        weights.push_back(weight);
    }


    for (Vehicle* v : v) {
        totalVehicleWeight += v->getCapacity();
    }

    if (totalVehicleWeight < totalWeight) {
        throw OverLimitVehicles();
    }


    for (int i = 0; i < n; i++) {

        int j;

        int min = c + 1, bi = 0;

        for (j = 0; j < aux.size(); j++) {
            if (aux[j]->getCapacity() >= weights[i] &&
                aux[j]->getCapacity() - weights[i] < min) {
                bi = j;
                min = aux[j]->getCapacity() - weights[i];
            }
        }


        int new_capacity = aux[bi]->getCapacity() - weights[i];
        aux[bi]->setCapacity(new_capacity);

        int index = -1;
        for (int i = 0; i < finalPaths.size(); i++) {
            if (finalPaths.at(i).first->getID() == aux[bi]->getID()) {
                index = i;
                break;
            }
        }



        vector<tuple<nodeInfo, vector<nodeInfo>>> deliveriesInVehicle;

        if (index != -1) {
            deliveriesInVehicle = finalPaths.at(index).second;
            finalPaths.erase(finalPaths.begin() + index);
        }



        deliveriesInVehicle.push_back(deliveries[i]);
        finalPaths.push_back(make_pair(aux[bi],deliveriesInVehicle));


    }


    return finalPaths;
}


vector<pair<Vehicle*, vector<tuple<nodeInfo, vector<nodeInfo>>>>>
divideVehicles(vector<Vehicle*> v, vector<tuple<nodeInfo, vector<nodeInfo>>> deliveries) {

    int capacityVehicle = v.at(0)->getCapacity();
    vector<pair<Vehicle*, vector<tuple<nodeInfo, vector<nodeInfo>>>>> res = bestFit(deliveries, deliveries.size(),
                                                    v.at(0)->getCapacity(), v);

    return res;
}

