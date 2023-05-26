cmake_minimum_required(VERSION 3.1.0)
project(RayTracing50 VERSION 0.1.0)

set(SUB_PROJECT_NAME RTKit1)

# find packages
find_package(Stb REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

# target
aux_source_directory(./asset ASSET_SRC)
aux_source_directory(./framework FRAMEWORK_SRC)
aux_source_directory(./material MATERIAL_SRC)
aux_source_directory(./math MATH_SRC)
aux_source_directory(./sampling SAMPLING_SRC)
aux_source_directory(./scene SCENE_SRC)

add_library(${SUB_PROJECT_NAME} STATIC 
                ${ASSET_SRC}
                ${FRAMEWORK_SRC}
                ${MATERIAL_SRC}
                ${MATH_SRC}
                ${SAMPLING_SRC}
                ${SCENE_SRC}
                )

target_include_directories(${SUB_PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${SUB_PROJECT_NAME}
                PUBLIC ${PROJECT_SOURCE_DIR}
            )

# libs
target_link_libraries(${SUB_PROJECT_NAME} PUBLIC 
                        glm::glm
                        SDL2::SDL2main SDL2::SDL2
                        tinyobjloader::tinyobjloader
                    )

