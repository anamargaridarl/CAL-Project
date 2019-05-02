#include <iostream>
#include "Graph.h"
#include "MutablePriorityQueue.h"

using namespace std;
int main() {
    std::cout << "Hello, World!" << std::endl;


    Vertex<int> A(12);
    cout << A.getPath() << endl;
    cout <<  A.getInfo() << endl;

    return 0;



}