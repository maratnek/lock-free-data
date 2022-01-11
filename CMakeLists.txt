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
conan_cmake_run(REQUIRES benchmark/1.6.0@ boost/1.78.0@
        GENERATORS cmake cmake_paths cmake_find_package
        BUILD missing
        BUILD_TYPE Release 
        BASIC_SETUP CMAKE_TARGETS
)

# lock free stack bench with leak and empty problem
add_executable(lf-stack-bench-leak lf-stack-bench.leak.cpp)
target_link_libraries(lf-stack-bench-leak CONAN_PKG::benchmark)

# lock free stack bench with leak 
add_executable(lf-stack-shared-bench-leak lf-stack-shared-bench.leak.cpp)
target_link_libraries(lf-stack-shared-bench-leak CONAN_PKG::benchmark)

# lock free stack bench atomic_shared_ptr 
# add_executable(lf-stack-atomic-shared-ptr-bench lf-stack-atomic-shared-ptr-bench.cpp)
# target_link_libraries(lf-stack-atomic-shared-ptr-bench CONAN_PKG::benchmark)

# reference counting 
add_executable(lf-stack-reference-counting lf-stack-reference-counting-bench.cpp)
target_link_libraries(lf-stack-reference-counting CONAN_PKG::benchmark)

# hazard 
add_executable(lf-stack-hazard-bench lf-stack-shared-hazard-bench.cpp)
target_link_libraries(lf-stack-hazard-bench CONAN_PKG::benchmark)

# boost
add_executable(lf-stack-boost-bench lf-stack-boost-bench.cpp)
target_link_libraries(lf-stack-boost-bench CONAN_PKG::benchmark CONAN_PKG::boost)

include(CTest)
enable_testing()
add_test(NAME lf-stack-bench COMMAND lf-stack-bench)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
