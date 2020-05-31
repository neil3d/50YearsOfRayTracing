cmake_minimum_required(VERSION 3.1.0)

project(RayTracingHistory)

set(SUB_PROJECT_NAME BasicPathTracer)

add_executable(${SUB_PROJECT_NAME} main.cpp
                DemoScene.h
                DirectLightingRenderer.h
                DirectLightingRenderer.cpp
                )

target_link_libraries(${SUB_PROJECT_NAME} MyFramework)