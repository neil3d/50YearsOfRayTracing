cmake_minimum_required(VERSION 3.1.0)
project(my_framework VERSION 0.1.0)

# set the C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find packages
find_package(SDL2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# target
aux_source_directory(./framework FRAMEWORK_SRC)
aux_source_directory(./material MATERIAL_SRC)
aux_source_directory(./math MATH_SRC)
aux_source_directory(./scene SCENE_SRC)

add_library(${PROJECT_NAME} STATIC 
                ${FRAMEWORK_SRC}
                ${MATERIAL_SRC}
                ${MATH_SRC}
                ${SCENE_SRC}
                )

target_include_directories(${PROJECT_NAME}
                PUBLIC ${PROJECT_SOURCE_DIR}
            )

# libs
target_link_libraries(${PROJECT_NAME} PUBLIC 
                        SDL2::SDL2main SDL2::SDL2-static
                        spdlog::spdlog spdlog::spdlog_header_only
                        glm
                    )

