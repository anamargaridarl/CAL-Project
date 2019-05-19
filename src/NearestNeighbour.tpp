#ifndef NEAREST_NEIGHBOUR_TPP_
#define NEAREST_NEIGHBOUR_TPP_

#include <algorithm>
#include <tuple>

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

/*
 * @param destinations: Todos os possiveis sitios para onde o carro pode ir
 * @ret O ponto para onde ele efetivamente vai e o trajeto que vai tomar até lá
 */
tuple<nodeInfo, vector<nodeInfo>> Graph<T>::getShortestPath(nodeInfo point, vector<nodeInfo> destinations)
{
    vector<nodeInfo> possiblePoints = getNearestPoints(point, destinations);

    double distance = -1;
    vector<nodeInfo> finalPath;
    nodeInfo node;
    for(nodeInfo endPoint : possiblePoints)
    {
        this.dijkstraShortestPath(point, endPoint);
        vector<nodeInfo> path = graph.getPath(point, endPoint);
        double cost = getCost(path);
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
        if(retrievalPoint == point.get(0))
        {
            deliveryPoints = point.get(1);
            break;
        }
    }

    return deliveryPoints;
}

template<class T>
vector<vector<T>> Graph<T>::nearestNeighbour(nodeInfo startPoint,  vector<tuple<nodeInfo, vector<nodeInfo>>> deliveries) {
    vector<nodeInfo> pointsAvailableAtStart;

    //initialValues
    for(tuple<nodeInfo, vector<nodeInfo>> nodes : deliveries)
    {
        pointsAvailableAtStart.push_back(nodes.get(0));
    }

    nodeInfo currentPosition = startPoint;
    vector<nodeInfo> route;

    while(!pointsAvailableAtStart.empty())
    {
        tuple<nodeInfo, vector<nodeInfo>> startPath = this.getShortestPath(currentPositionStart, pointsAvailableAtStart);

        route.insert(route.end(), startPath.get(1).begin(), startPath.get(1).end());
        currentPosition = startPath.get(0);
        
        pointsAvailableAtStart.erase(find(pointsAvailableAtStart.begin(), pointsAvailableAtStart.end(), currentPosition));
        vector<nodeInfo> newPoints = getDeliveryPoints(currentPosition, deliveries);
        pointsAvailableAtStart.insert(pointsAvailableAtStart.end(), newPoints.begin(), newPoints.end());
    }

    return route;
}

#endif
