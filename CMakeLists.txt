cmake_minimum_required(VERSION 3.9)
project(Project)

set(CMAKE_CXX_STANDARD 11)

add_executable(Project src/main.cpp
         src/Graph.h
        src/MutablePriorityQueue.h
        src/Option.cpp
        src/Option.h
       )
