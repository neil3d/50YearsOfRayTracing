cmake_minimum_required(VERSION 3.1.0)
project(RayTracing50 VERSION 0.1.0)

set(SUB_PROJECT_NAME WhittedRayTracer)

add_executable(${SUB_PROJECT_NAME} main.cpp
                BoxScene.h
                DemoScene.h
                Light.h
                Material.h
                WhittedRayTracer.h
                WhittedRayTracer.cpp
                TestSceneBase.h
                TestScene.h
                )

target_link_libraries(${SUB_PROJECT_NAME} RTKit1)