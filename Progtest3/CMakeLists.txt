cmake_minimum_required(VERSION 3.7)
project(Progtest3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Vector.h Vertex.h Edge.h Heap.h EdgePair.h)
add_executable(Progtest3 ${SOURCE_FILES})