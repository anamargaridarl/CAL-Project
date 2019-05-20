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
    for(int i = 0; i < path.size() - 1; i++)
    {
        Vertex* v1 = findVertex(path[i]);
        Vertex* v2 = findVertex(path[i+1]);

        cost += v1->getEdge(v2).weight;
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
        dijkstraShortestPath(point, endPoint);
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
    dijkstraShortestPath(currentPosition, startPoint);
    vector<nodeInfo> finalPath = getPath(currentPosition, startPoint);
    route.insert(route.end(), finalPath.begin(), finalPath.end());

    return twoOpt(route, deliveries);
}

bool checkValidOrder(vector<nodeInfo> path, vector<tuple<nodeInfo, vector<nodeInfo>>> pointsOfInterest)
{
    vector<nodeInfo> retrievals;
    vector<nodeInfo> others;
    for(tuple<nodeInfo, vector<nodeInfo>> combo : pointsOfInterest)
    {
        retrievals.push_back(get<0>(combo));
    }

    for(nodeInfo node : path)
    {
        auto it = find(retrievals.begin(), retrievals.end(), node);
        if(it != retrievals.end())
        {
            long index = std::distance(retrievals.begin(), it);
            vector<nodeInfo> deliveries = get<1>(pointsOfInterest[index]);
            for(nodeInfo delivery : deliveries)
                if(find(others.begin(), others.end(), delivery) != others.end()) return false;
        }
        else others.push_back(node);
    }

    return true;
}

bool Graph::checkValidPath(vector<nodeInfo> path, double &previousCost)
{
    double cost = 0;
    for(int i = 0; i < path.size() - 1; i++)
    {
        Vertex* v1 = findVertex(path[i]);
        Vertex* v2 = findVertex(path[i+1]);

        if(v1->getEdge(v2).dest == nullptr)
            return false;

        cost += v1->getEdge(v2).weight;
    }
    if(cost < previousCost)
    {
        previousCost = cost;
        return true;
    }
    else return false;
}

vector<nodeInfo> Graph::twoOpt(vector<nodeInfo> currentPath, vector<tuple<nodeInfo, vector<nodeInfo>>> deliveries)
{
    size_t numberOfNodes = currentPath.size();
    double minCost = getPathCost(currentPath);
    bool tryAgain;

    do {
        tryAgain = false;
        for(int i = 0; i < numberOfNodes; i++)
        {
            for(int j = i+1; j < numberOfNodes-1; j++)
            {
                vector<nodeInfo> tmpPath;
                twoOptSwap(i, j, tmpPath, currentPath);
                if(checkValidOrder(tmpPath, deliveries) && checkValidPath(tmpPath, minCost))
                {
                    currentPath = tmpPath;
                    tryAgain = true;
                    break;
                }
            }
            if(tryAgain) break;
        }
    } while(tryAgain);

    return currentPath;
}

void Graph::twoOptSwap(int i, int j, vector<nodeInfo> &tmpPath, vector<nodeInfo> currentPath)
{
    //add from route[0] to route[i-1]
    for ( int x = 0; x <= i - 1; x++)
    {
        tmpPath.push_back(currentPath[x]);
    }
    //add from route[i] to route[j] in reverse order
    for ( int x = j; x >= i; x--)
    {
        tmpPath.push_back(currentPath[x]);
    }
    //add from route[j+1] to end
    for ( int x = j + 1; x < currentPath.size(); x++)
    {
        tmpPath.push_back(currentPath[x]);
    }
}