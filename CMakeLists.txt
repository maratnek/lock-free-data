cmake_minimum_required(VERSION 3.0.0)
project(lfdata VERSION 0.1.0)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(REQUIRES benchmark/1.5.6@ 
        GENERATORS cmake cmake_paths cmake_find_package
        BUILD_TYPE Release 
        BASIC_SETUP CMAKE_TARGETS
)

include(CTest)
enable_testing()

add_executable(lfdata main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
