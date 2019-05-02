cmake_minimum_required(VERSION 3.9)
project(Project)

set(CMAKE_CXX_STANDARD 11)

<<<<<<< Updated upstream
add_executable(Project src/main.cpp
         src/Graph.h
        src/MutablePriorityQueue.h
       )
=======
add_executable(Project src/main.cpp src/Option.cpp src/Option.h)
>>>>>>> Stashed changes
