cmake_minimum_required(VERSION 3.3)
project(KnapsackProblem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp knapsack.h item.h knapsack.cpp tester.cpp tester.h)
add_executable(KnapsackProblem ${SOURCE_FILES})