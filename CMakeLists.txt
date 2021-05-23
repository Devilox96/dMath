cmake_minimum_required(VERSION 3.15)
project(dMath)

set(CMAKE_CXX_STANDARD 17)
set(
        CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror"
)
add_subdirectory(Core)
add_subdirectory(NumerCalc)

set(dMathLibs
        dMathCoreLib
        dMathNumerCalcLib)

add_executable(dMath main.cpp)
target_link_libraries(dMath ${dMathLibs})
