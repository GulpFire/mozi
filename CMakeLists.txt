cmake_minimum_required(VERSION 3.0.2)
project(mozi)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

include(${CMAKE_CURRENT_SOURCE_DIR}/build_options.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/project_settings.cmake)

add_subdirectory(core)
if(BUILD_TEST)
add_subdirectory(test)
endif(BUILD_TEST)
