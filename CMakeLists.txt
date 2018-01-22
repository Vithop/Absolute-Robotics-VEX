cmake_minimum_required(VERSION 3.4)
project(Vex_robot_1_0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Vex_robot_1_0 ${SOURCE_FILES})