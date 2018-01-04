cmake_minimum_required(VERSION 3.7)
project(Progtest2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wno-long-long -Werror")
set(SOURCE_FILES main.cpp )#vojta.cpp)
add_executable(Progtest2 ${SOURCE_FILES})