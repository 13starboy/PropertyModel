cmake_minimum_required(VERSION 3.26)

project(pm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    src 
    src/graph 
    src/algorithm
)

set(PROJECT_SOURCES 
    src/graph/variable.cpp
    src/graph/method.cpp
    src/graph/constraint.cpp
    src/graph/constraint_graph.cpp
    src/graph/utils.cpp

    src/algorithm/blue.cpp

    src/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

