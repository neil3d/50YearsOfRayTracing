cmake_minimum_required(VERSION 3.1.0)
project(RayTracing50 VERSION 0.1.0)

set(SUB_PROJECT_NAME RayCasting)

add_executable(${SUB_PROJECT_NAME} code/main.cpp
                code/RayCastingRenderer.h
                code/RayCastingRenderer.cpp
                )

target_link_libraries(${SUB_PROJECT_NAME} RTKit1)