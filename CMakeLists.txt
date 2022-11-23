cmake_minimum_required(VERSION 3.23)
project(prm_exercise)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

add_executable(prm_exercise main.cpp prm/Node.cpp prm/Node.h prm/Graph.cpp prm/Graph.h prm/Edge.cpp prm/Edge.h astar/Astar.cpp astar/Astar.h)

target_link_libraries(prm_exercise ${OpenCV_LIBS})