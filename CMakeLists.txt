cmake_minimum_required(VERSION 3.26)

project(pm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)
set(PROJECT_SOURCES 
    src/variable.cpp
    src/method.cpp
    src/constraint.cpp
    src/constraint_graph.cpp

    src/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

