project(football_team)
cmake_minimum_required(VERSION 2.8)

set(SOURCE main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(${PROJECT_NAME} ${SOURCE})

