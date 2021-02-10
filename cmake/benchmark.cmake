# the following code to fetch googletest
# is inspired by and adapted after https://crascit.com/2015/07/25/cmake-gtest/
# download and unpack googletest at configure time

macro(fetch_benchmark _download_module_path _download_root)
    set(BENCHMARK_DOWNLOAD_ROOT ${_download_root})
    configure_file(
        ${_download_module_path}/benchmark-download.cmake
        ${_download_root}/CMakeLists.txt
        @ONLY
        )
    unset(BENCHMARK_DOWNLOAD_ROOT)

    execute_process(
        COMMAND
            "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY
            ${_download_root}
        )
    execute_process(
        COMMAND
            "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY
            ${_download_root}
        )

    add_subdirectory(
        ${_download_root}/benchmark-src
        ${_download_root}/benchmark-build
    )
    include_directories(
        ${_download_root}/benchmark-src/include
    )
endmacro()
