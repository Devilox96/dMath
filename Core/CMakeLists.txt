project(dMathCoreLib)

set(Source
        dVectors.cpp
        dVector.h
        dQuaternion.h
        dMatrix.h)

add_library(dMathCoreLib ${Source})

include(../cmake/googletest.cmake)
fetch_googletest(
        ${PROJECT_SOURCE_DIR}/../cmake
        ${PROJECT_BINARY_DIR}/../googletest
)

include(../cmake/benchmark.cmake)
fetch_benchmark(
        ${PROJECT_SOURCE_DIR}/../cmake
        ${PROJECT_BINARY_DIR}/../benchmark
)

enable_testing()
add_subdirectory(test)
add_subdirectory(benchmark)
