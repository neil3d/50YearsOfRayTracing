cmake_minimum_required(VERSION 3.1.0)

project(RayTracingHistory)

set(SUB_PROJECT_NAME DistributedRayTracer)

add_executable(${SUB_PROJECT_NAME} main.cpp
                AreaLight.h
                BallMaterial.h
                BilliardScene.h
                DistributedRayTracer.h
                DistributedRayTracer.cpp
                TableMaterial.h
                )

target_link_libraries(${SUB_PROJECT_NAME} MyFramework)