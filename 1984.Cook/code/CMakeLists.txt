cmake_minimum_required(VERSION 3.1.0)

project(RayTracingHistory)

set(SUB_PROJECT_NAME DistributedRayTracer)

add_executable(${SUB_PROJECT_NAME} main.cpp
                AreaLight.h
                Material.h
                BilliardSceneClone.h
                BilliardSceneDemo.h
                DistributedRayTracer.h
                DistributedRayTracer.cpp
                MySceneWithLight.h
                )

target_link_libraries(${SUB_PROJECT_NAME} MyFramework)