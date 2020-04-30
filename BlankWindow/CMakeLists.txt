cmake_minimum_required(VERSION 3.1.0)
project(BlankWindow VERSION 0.1.0)

find_package(SDL2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_executable(BlankWindow main.cpp)

target_link_libraries(BlankWindow PRIVATE 
                        SDL2::SDL2main SDL2::SDL2-static
                        spdlog::spdlog spdlog::spdlog_header_only
                    )

