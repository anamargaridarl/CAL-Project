//
// Created by anamargaridarl on 22-05-2019.
//


#ifndef CAL_PROJECT_DIVIDEVEHICLES_H
#define CAL_PROJECT_DIVIDEVEHICLES_H

#include <vector>
#include <tuple>
#include "Vehicle.h"

std::vector<std::pair<Vehicle*, std::vector<std::tuple<nodeInfo,std::vector<nodeInfo>>>>> bestFit(std::vector<std::tuple<nodeInfo, std::vector<nodeInfo>>> deliveries, int n, int c, std::vector<Vehicle*> v);

std::vector<std::pair<Vehicle*, std::vector<std::tuple<nodeInfo, std::vector<nodeInfo>>>>>
divideVehicles(std::vector<Vehicle*> v, std::vector<std::tuple<nodeInfo, std::vector<nodeInfo>>> deliveries);

#endif //CAL_PROJECT_DIVIDEVEHICLES_H
