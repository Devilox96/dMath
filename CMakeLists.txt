cmake_minimum_required(VERSION 3.9)
project(dMath)

set(CMAKE_CXX_STANDARD 17)

set(QtPath /home/devilox/DeviloxDocuments/ProgTools/Qt/5.12.0/gcc_64/lib/cmake)
set(QtPackages
        Qt5::Core
        Qt5::Widgets
        Qt5::Network)
set(QtComponents
        Core
        Widgets
        Network)
find_package(Qt5 COMPONENTS ${QtComponents} PATHS ${QtPath} NO_DEFAULT_PATH)
#---Qt---#

add_subdirectory(Core)
add_subdirectory(NumerCalc)

set(dMathLibs
        dMathCoreLib
        dMathNumerCalcLib)

add_executable(dMath main.cpp)
target_link_libraries(dMath
        ${dMathLibs}
        ${QtPackages})