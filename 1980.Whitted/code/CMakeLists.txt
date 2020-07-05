cmake_minimum_required(VERSION 3.1.0)

project(RayTracingHistory)

set(SUB_PROJECT_NAME WhittedRayTracer)

add_executable(${SUB_PROJECT_NAME} main.cpp
                DemoScene.h
                Light.h
                Material.h
                WhittedRayTracer.h
                WhittedRayTracer.cpp
                TestSceneBase.h
                TestScene.h
                )

target_link_libraries(${SUB_PROJECT_NAME} MyFramework)