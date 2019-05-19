cmake_minimum_required(VERSION 3.9)
project(Project)

set(CMAKE_CXX_STANDARD 11)

add_executable(Project src/main.cpp
        src/Graph.cpp
        src/Graph.h
        src/Vertex.cpp
        src/Vertex.h
        src/Edge.cpp
        src/Edge.h
        src/MutablePriorityQueue.h
        src/Option.cpp
        src/Option.h
        src/Vehicle.cpp
        src/Vehicle.h
        src/Menu.cpp
        src/Menu.h
        src/GraphImporter.cpp
        src/GraphImporter.h
        src/NearestNeighbour.tpp
        src/GraphViewer/connection.h
        src/GraphViewer/connection.cpp
        src/GraphViewer/edgetype.h
        src/GraphViewer/graphviewer.h
        src/GraphViewer/graphviewer.cpp src/A-star.tpp)

