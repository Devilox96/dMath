add_executable(
    unit_tests
    main.cpp
    vector.cpp
    matrix.cpp
    )

target_link_libraries(
    unit_tests
    gtest_main
    dMathCoreLib
    )

add_test(
  NAME
    unit
  COMMAND
    ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
  )
