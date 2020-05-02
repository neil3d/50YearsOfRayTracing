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
add_library(${PROJECT_NAME} STATIC
                framework/MyApp.h
                framework/MyApp.cpp
                framework/MyException.h
                framework/MyRenderer.h
                framework/MyRenderer.cpp
                )

target_include_directories( ${PROJECT_NAME}
                PUBLIC ${PROJECT_SOURCE_DIR}
            )

# libs
target_link_libraries(${PROJECT_NAME} PUBLIC 
                        SDL2::SDL2main SDL2::SDL2-static
                        spdlog::spdlog spdlog::spdlog_header_only
                        glm
                    )

