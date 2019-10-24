project(dMathCoreLib)

set(Source
        dVectors.cpp dMatrix.h)

add_library(dMathCoreLib ${Source})

include(../cmake/googletest.cmake)
fetch_googletest(
        ${PROJECT_SOURCE_DIR}/../cmake
        ${PROJECT_BINARY_DIR}/../googletest
)

enable_testing()
add_subdirectory(test)
