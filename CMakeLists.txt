cmake_minimum_required(VERSION 3.0.0)
project(RadiPako VERSION 2.0.0.0)

file(GLOB src "./src/*.cpp")

file(GLOB rpklib "./src/RadiPako.cpp")

add_executable(RadiPako main.cpp ${src})
add_library(RPK STATIC ${rpklib})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
