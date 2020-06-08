cmake_minimum_required(VERSION 3.9)
project(dMath)

set(CMAKE_CXX_STANDARD 17)

#set(QtPath /home/devilox/DeviloxDocuments/ProgTools/Qt/5.12.0/gcc_64/lib/cmake)
#set(QtPackages
#        Qt5::Core
#        Qt5::Widgets
#        Qt5::Network)
#set(QtComponents
#        Core
#        Widgets
#        Network)
#find_package(Qt5 COMPONENTS ${QtComponents} PATHS ${QtPath} NO_DEFAULT_PATH)
#---Qt---#

find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(SDL2 REQUIRED FATAL_ERROR)
find_package(OpenMP REQUIRED)

add_subdirectory(Core)
add_subdirectory(NumerCalc)

set(dMathLibs
        dMathCoreLib
        dMathNumerCalcLib)

add_executable(dMath main.cpp Renderer.cpp)
target_include_directories(dMath PRIVATE Vulkan::Vulkan)
target_include_directories(dMath PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(dMath
        ${dMathLibs}
        ${QtPackages}
        Vulkan::Vulkan
        ${SDL2_LIBRARIES}
        OpenMP::OpenMP_CXX)