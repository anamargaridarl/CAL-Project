//
// Created by anamargaridarl on 20-05-2019.
//
#include "Graph.h"
#include "Vehicle.h"
#include <algorithm>
#include <tuple>
#include <iostream>

using namespace std;

class OverLimitVehicles
{
public:
    OverLimitVehicles(){};
    virtual std::string what() const{
        return "Request cant be successfully implemented: Quantity of goods is over the limits of the available vehicles";
    }

};

bool myfunction (Vehicle i,Vehicle j) {
    return (i.getCapacity()>j.getCapacity()); }


vector<tuple<nodeInfo, vector<nodeInfo>>> divideVehicles(vector<Vehicle>v ,vector<tuple<nodeInfo, vector<nodeInfo>>> deliveries )
{
    vector< tuple<nodeInfo, int> >weightPerCollectPoint;
    int totalWeight = 0;

    //Calculate weghts for each collectPoint
    for(int i=0 ; i < deliveries.size(); i++)
    {
        vector<nodeInfo>deliveryPoints = get<1>(deliveries.at(i));
        int weight = 0;
        for(int n = 0; n < deliveryPoints.size(); n++)
        {
            weight += deliveryPoints.at(n).quantity;
        }

        totalWeight += weight;
        nodeInfo collectPoint = get<0>(deliveries.at(i));
        tuple<nodeInfo, int> t(collectPoint,weight);
        weightPerCollectPoint.push_back(t);
    }

    int totalVehicleWeight = 0;
    for(Vehicle v : v)
    {
        totalVehicleWeight += v.getCapacity();
    }

    if(totalVehicleWeight < totalWeight)
    {
        throw OverLimitVehicles();
    }

    sort(v.begin(), v.end(),myfunction);





}

