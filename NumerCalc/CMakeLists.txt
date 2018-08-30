project(dMathNumerCalcLib)

set(dMathCoreDir ../Core)
set(dMathCoreSource
        ${dMathCoreDir}/dVectors.cpp)

set(Source
        dRungeKutta.cpp)

add_library(dMathNumerCalcLib ${Source} ${dMathCoreSource})
target_link_libraries(dMathNumerCalcLib dMathCoreLib)
target_include_directories(dMathNumerCalcLib PUBLIC ${dMathCoreDir})