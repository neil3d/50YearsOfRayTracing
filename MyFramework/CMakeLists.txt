cmake_minimum_required(VERSION 3.1.0)
project(BlankWindow VERSION 0.1.0)

# set the C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find packages
find_package(SDL2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# target
add_executable(BlankWindow 
                main.cpp
                MyApp.h
                MyApp.cpp
                MyRenderer.h
                MyRenderer.cpp
                MyException.h
                )

# libs
target_link_libraries(BlankWindow PRIVATE 
                        SDL2::SDL2main SDL2::SDL2-static
                        spdlog::spdlog spdlog::spdlog_header_only
                        glm
                    )

