cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

if (WIN32)
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

add_subdirectory(osupp)
add_subdirectory(osuppy)
add_subdirectory(executable)
