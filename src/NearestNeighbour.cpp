#include "Graph.h"
#include <algorithm>
#include <tuple>
#include <iostream>

int numberOfNearest = 3;

vector<nodeInfo> getNearestPoints(nodeInfo start, vector<nodeInfo> allPoints)
{
    vector<nodeInfo> nearestPoints;
    for(nodeInfo node : allPoints)
    {
        if(nearestPoints.size() < numberOfNearest)
        {
            nearestPoints.push_back(node);
            continue;
        }

        int distance = start.distance(node);
        double longestDistance = 0;
        int number = 0;
        for(int i = 0; i < nearestPoints.size(); i++)
        {
            int dist = start.distance(nearestPoints[i]);
            if(dist > longestDistance)
            {
                longestDistance = dist;
                number = i;
            }
        }

        if(distance < longestDistance)
        {
            nearestPoints.erase(nearestPoints.begin() + number);
            nearestPoints.push_back(node);
        }
    }

    return nearestPoints;
}

double Graph::getPathCost(vector<nodeInfo> path) {
    double cost = 0;
    for(nodeInfo node : path)
    {
        cost += getCost(node);
    }
    return cost;
}

/*
 * @param destinations: Todos os possiveis sitios para onde o carro pode ir
 * @ret O ponto para onde ele efetivamente vai e o trajeto que vai tomar até lá
 */
tuple<nodeInfo, vector<nodeInfo>> Graph::getShortestPath(nodeInfo point, vector<nodeInfo> destinations)
{
    vector<nodeInfo> possiblePoints = getNearestPoints(point, destinations);

    double distance = -1;
    vector<nodeInfo> finalPath;
    nodeInfo node;
    for(nodeInfo endPoint : possiblePoints)
    {
        aStarShortestPath(point, endPoint);
        //dijkstraShortestPath(point, endPoint);
        vector<nodeInfo> path = getPath(point, endPoint);
        double cost = getPathCost(path);
        if(distance == -1 || cost < distance)
        {
            distance = cost;
            finalPath = path;
            node = endPoint;
        }
    }

    return std::make_tuple(node, finalPath);
}

vector<nodeInfo> getDeliveryPoints(nodeInfo retrievalPoint, vector<tuple<nodeInfo, vector<nodeInfo>>> points)
{
    vector<nodeInfo> deliveryPoints;
    for(auto point : points)
    {
        if(retrievalPoint == std::get<0>(point))
        {
            deliveryPoints = std::get<1>(point);
            break;
        }
    }

    return deliveryPoints;
}

vector<nodeInfo> Graph::nearestNeighbour(nodeInfo startPoint,  vector<tuple<nodeInfo, vector<nodeInfo>>> deliveries) {
    vector<nodeInfo> pointsAvailableAtStart;

    //initialValues
    for(tuple<nodeInfo, vector<nodeInfo>> nodes : deliveries)
    {
        pointsAvailableAtStart.push_back(get<0>(nodes));
    }

    nodeInfo currentPosition = startPoint;
    vector<nodeInfo> route;

    while(!pointsAvailableAtStart.empty())
    {
        tuple<nodeInfo, vector<nodeInfo>> shortestPath = getShortestPath(currentPosition, pointsAvailableAtStart);

        route.insert(route.end(), get<1>(shortestPath).begin(), get<1>(shortestPath).end());
        currentPosition = get<0>(shortestPath);

        pointsAvailableAtStart.erase(find(pointsAvailableAtStart.begin(), pointsAvailableAtStart.end(), currentPosition));
        vector<nodeInfo> newPoints = getDeliveryPoints(currentPosition, deliveries);
        pointsAvailableAtStart.insert(pointsAvailableAtStart.end(), newPoints.begin(), newPoints.end());
    }
    //dijkstraShortestPath(currentPosition, startPoint);
    aStarShortestPath(currentPosition, startPoint);

    vector<nodeInfo> finalPath = getPath(currentPosition, startPoint);
    route.insert(route.end(), finalPath.begin(), finalPath.end());

    return route;
}
