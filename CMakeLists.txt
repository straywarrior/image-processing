cmake_minimum_required (VERSION 2.8)

project(imageprocess)

if(NOT MSVC)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions("-g")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_definitions("-O2")
    endif()
    add_definitions("-Wall")
    add_definitions("-std=c++11")
endif()


option(WITH_OpenCV "Built with OpenCV" OFF)
if (WITH_OpenCV)
    find_package(OpenCV REQUIRED)
    add_definitions("-DHAVE_OPENCV")
    include_directories("${OpenCV_INCLUDE_DIRS}")
endif (WITH_OpenCV)

add_subdirectory(source)
add_subdirectory(test)