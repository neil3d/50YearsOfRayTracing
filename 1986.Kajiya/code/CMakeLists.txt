cmake_minimum_required(VERSION 3.1.0)

project(RayTracingHistory)

set(SUB_PROJECT_NAME MonteCarloPathTracer)

add_executable(${SUB_PROJECT_NAME} main.cpp
                CornellBoxScene.h
                DemoScene.h
                DiffuseMaterial.h
                MaterialBase.h
                MonteCarloPathTracer.h
                MonteCarloPathTracer.cpp
                )

target_link_libraries(${SUB_PROJECT_NAME} MyFramework)