cmake_minimum_required(VERSION 3.1.0)
project(RayTracing50 VERSION 0.1.0)

# set the C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# sub projects
add_subdirectory(RTKit1)
add_subdirectory(1968.Appel)
add_subdirectory(1980.Whitted)
add_subdirectory(1984.Cook)
add_subdirectory(1986.Kajiya)
