cmake_minimum_required(VERSION 3.0.0)

project(RayTracing50 VERSION 0.1.0)
set(SUB_PROJECT_NAME AnalyticDirectLighting)

aux_source_directory(./code MY_SRC)

add_executable(AnalyticDirectLighting ${MY_SRC})

target_link_libraries(${SUB_PROJECT_NAME} RTKit2)
