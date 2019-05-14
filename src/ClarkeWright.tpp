#ifndef CLARKE_WRIGHT_TPP_
#define CLARKE_WRIGHT_TPP_

#include <algorithm>
#include <map>

template<class T>
struct PointLink {
    T point1;
    T point2;
};

template<class T>
struct Saving {
    PointLink<T> link;
    int value;
};

template<class T>
bool savingBetterThan(Saving<T> s1, Saving<T> s2)
{
    return (s1.value < s2.value);
}

template<class T>
vector<vector<T>> Graph<T>::clarkeWright(vector<T> points) {
    vector<vector<T>> routes;

    vector<Saving<T>> savings;
    for(int i=0; i< points.size(); i++) {
        for(int j=0; j< points.size(); j++){
            if(i==j)
                continue;
            dijkstraShortestPath(points.at(i), points.at(j));
            PointLink<T> link {points.at(i), points.at(j)};
            Saving<T> saving {link, getCost(points.at(j))};
            savings.push_back(saving);
        }
    }

    sort(savings.begin(), savings.end(), savingBetterThan<T>);

    for (Saving<T> saving : savings) {
        cout << ((nodeInfo)saving.link.point1).nodeID << "->" << ((nodeInfo)saving.link.point2).nodeID << ": " << saving.value << endl;
    }

    return routes;
}

#endif
