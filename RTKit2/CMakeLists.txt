cmake_minimum_required(VERSION 3.0.0)
project(CSE168xHomework VERSION 0.1.0)

set(SUB_PROJECT_NAME RTKit2)

# dependencies
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(embree 3 CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)

# source files
aux_source_directory(./asset ASSET_SRC)
aux_source_directory(./external EXTERNAL_SRC)
aux_source_directory(./framework FRAMEWORK_SRC)
aux_source_directory(./material MATERIAL_SRC)
aux_source_directory(./sampling SAMPLING_SRC)
aux_source_directory(./scene SCENE_SRC)
aux_source_directory(./utility UTILITY_SRC)

add_library(RTKit2 
                ${ASSET_SRC}
                ${EXTERNAL_SRC}
                ${FRAMEWORK_SRC}
                ${MATERIAL_SRC}
                ${SAMPLING_SRC}
                ${SCENE_SRC}
                ${UTILITY_SRC}
                )

target_include_directories(RTKit2 PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(RTKit2 PUBLIC
                        glm::glm
                        embree
                        SDL2::SDL2main SDL2::SDL2
                        tinyobjloader::tinyobjloader
                    )

target_include_directories(${SUB_PROJECT_NAME}
                    PUBLIC ${PROJECT_SOURCE_DIR}
                )


