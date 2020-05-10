cmake_minimum_required(VERSION 3.1.0)

project(ray_tracing_history)

set(SUB_PROJECT_NAME WhittedRayTracer)

add_executable(${SUB_PROJECT_NAME} main.cpp
                WhittedRayTracer.h
                WhittedRayTracer.cpp
            )

target_link_libraries(${SUB_PROJECT_NAME} MyFramework)