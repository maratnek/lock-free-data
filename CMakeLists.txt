cmake_minimum_required(VERSION 3.0.0)
project(lfdata VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_REQUIRED ON)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(REQUIRES benchmark/1.6.0@ 
        GENERATORS cmake cmake_paths cmake_find_package
        BUILD missing
        BUILD_TYPE Release 
        BASIC_SETUP CMAKE_TARGETS
)

add_executable(lfdata main.cpp)

# lock free stack bench
add_executable(lf-stack-bench lf-stack-bench.cpp)
target_link_libraries(lf-stack-bench CONAN_PKG::benchmark)

include(CTest)
enable_testing()
add_test(NAME lf-stack-bench COMMAND lf-stack-bench)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
