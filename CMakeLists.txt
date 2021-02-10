cmake_minimum_required(VERSION 3.9)
project(dMath)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Core)
add_subdirectory(NumerCalc)

set(dMathLibs
        dMathCoreLib)
#        dMathNumerCalcLib)

add_executable(dMath main.cpp main.cpp)
target_link_libraries(dMath ${dMathLibs})
