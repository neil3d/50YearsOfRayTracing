cmake_minimum_required(VERSION 3.1.0)
project(RayTracing50 VERSION 0.1.0)

set(SUB_PROJECT_NAME MonteCarloPathTracing)

aux_source_directory(./code CODE_FILES)
add_executable(${SUB_PROJECT_NAME} ${CODE_FILES})

target_link_libraries(${SUB_PROJECT_NAME} RTKit1)