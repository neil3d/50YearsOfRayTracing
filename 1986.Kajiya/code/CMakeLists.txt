cmake_minimum_required(VERSION 3.1.0)

project(RayTracingHistory)

set(SUB_PROJECT_NAME MonteCarloPathTracer)

add_executable(${SUB_PROJECT_NAME} main.cpp
                DemoScene.h
                SimpleMonteCarloApproach.h
                SimpleMonteCarloApproach.cpp
                LambertianMaterial.h
                MaterialBase.h
                )

target_link_libraries(${SUB_PROJECT_NAME} MyFramework)