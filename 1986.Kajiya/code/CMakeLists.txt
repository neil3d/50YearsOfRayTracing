cmake_minimum_required(VERSION 3.1.0)

project(RayTracingHistory)

set(SUB_PROJECT_NAME MonteCarloPathTracer)

add_executable(${SUB_PROJECT_NAME} main.cpp
                AreaLight.h
                CornellBoxScene.h
                DemoScene.h
                DiffuseMaterial.h
                GGXMaterial.h
                KajiyaScene.h
                LambertianMaterial.h
                MaterialBase.h
                MonteCarloPathTracer.h
                MonteCarloPathTracer.cpp
                MoriKnobScene.h
                MySceneWithLights.h
                ParallelogramLight.h
                PhongMaterial.h
                SkyLight.h
                )

target_link_libraries(${SUB_PROJECT_NAME} MyFramework)