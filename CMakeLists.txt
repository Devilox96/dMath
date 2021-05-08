cmake_minimum_required(VERSION 3.9)
project(dMath)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(SDL2 REQUIRED FATAL_ERROR)
find_package(OpenMP REQUIRED)

add_subdirectory(Core)
add_subdirectory(NumerCalc)

set(dMathLibs
        dMathCoreLib
        dMathNumerCalcLib)

add_executable(dMath main.cpp)
target_link_libraries(dMath ${dMathLibs})