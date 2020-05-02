cmake_minimum_required(VERSION 3.1.0)

project(whitted)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} my_framework)