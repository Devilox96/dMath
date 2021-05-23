project(dMathNumerCalcLib)

set(dMathCoreDir ../Core/)
set(dMathCoreSource
        ${dMathCoreDir}/Vector.cpp)

set(Source
        dRungeKutta.cpp
        dLaxFriedrichs1D.cpp
        dLaxFriedrichs2D.cpp
        dLeapFrog1D.cpp
        dLeapFrog2D.cpp
        dRichtmyer1D.cpp
        dRichtmyer2D.cpp
        dWENO2D.cpp)

add_library(dMathNumerCalcLib ${Source} ${dMathCoreSource})
target_link_libraries(dMathNumerCalcLib dMathCoreLib)
target_include_directories(dMathNumerCalcLib PUBLIC ${dMathCoreDir})
