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


vector<tuple<Vehicle*, vector<int>>> bestFit(vector<int> weight, int n, int c, vector<Vehicle*> v) {

    vector<tuple<Vehicle*, vector<int>>> res;

    vector<Vehicle*> aux = v;

    for (int i = 0; i < n; i++) {

        int j;

        int min = c + 1, bi = 0;

        for (j = 0; j < aux.size(); j++) {
            if (aux[j]->getCapacity() >= weight[i] &&
                aux[j]->getCapacity() - weight[i] < min) {
                bi = j;
                min = aux[j]->getCapacity() - weight[i];
            }
        }

        int new_capacity = aux[bi]->getCapacity() - weight[i];
        aux[bi]->setCapacity(new_capacity);

        int index = -1;
        for (int i = 0; i < res.size(); i++) {
            if (get<0>(res.at(i))->getID() == aux[bi]->getID()) {
                index = i;
                break;
            }
        }

        vector<int> weightsInVector;

        if (index != -1) {
            weightsInVector = get<1>(res.at(i));
            res.erase(res.begin() + index);
        }

        weightsInVector.push_back(weight[i]);
        tuple<Vehicle*, vector<int>> t(aux[bi], weightsInVector);
        res.push_back(t);


    }

    return res;
}


vector<pair<Vehicle*, vector<tuple<nodeInfo, vector<nodeInfo>>>>>
divideVehicles(vector<Vehicle*> v, vector<tuple<nodeInfo, vector<nodeInfo>>> deliveries) {

    vector<int> weightPerCollectPoint;

    int totalWeight = 0;

    //Calculate weghts for each collectPoint
    for (int i = 0; i < deliveries.size(); i++) {

        vector<nodeInfo> deliveryPoints = get<1>(deliveries.at(i));
        int weight = 0;

        for (int n = 0; n < deliveryPoints.size(); n++) {
            weight += deliveryPoints.at(n).quantity;
        }

        totalWeight += weight;
        weightPerCollectPoint.push_back(weight);
    }

    int totalVehicleWeight = 0;
    for (Vehicle* v : v) {
        totalVehicleWeight += v->getCapacity();
    }

    if (totalVehicleWeight < totalWeight) {
        throw OverLimitVehicles();
    }


    sort(weightPerCollectPoint.begin(), weightPerCollectPoint.end(), myfunction);

    int capacityVehicle = v.at(0)->getCapacity();
    vector<tuple<Vehicle*, vector<int>>> a = bestFit(weightPerCollectPoint, weightPerCollectPoint.size(),
                                                    v.at(0)->getCapacity(), v);

    vector<pair<Vehicle*, vector<tuple<nodeInfo, vector<nodeInfo>>>>> finalPaths;


    for (int i = 0; i < deliveries.size(); i++) {

        vector<nodeInfo> deliveryPoints = get<1>(deliveries.at(i));
        int weight = 0;

        for (int n = 0; n < deliveryPoints.size(); n++) {
            weight += deliveryPoints.at(n).quantity;
        }

        int flag = 0;
        for (tuple<Vehicle*, vector<int>> v : a) {
            if (flag == 1)
                break;

            for (int weightsinVehicles : get<1>(v)) {
                if (weightsinVehicles == weight && flag == 0) {
                    get<0>(v)->setCapacity(capacityVehicle);
                    finalPaths.push_back(std::make_pair(get<0>(v), deliveries));
                    flag = 1;
                    break;
                }
            }
        }


    }
    return finalPaths;
}

