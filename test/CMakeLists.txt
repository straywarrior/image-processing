option(BUILD_TEST "Build test cases. (Google Test required)" OFF)

if (NOT BUILD_TEST)
    return()
endif (NOT BUILD_TEST)

set(GTEST_ROOT_DIR "${CMAKE_PREFIX_PATH}" CACHE PATH "The directory where gtest is installed.")
message(${GTEST_ROOT_DIR})
find_path(GTEST_INCLUDE_DIR "gtest/gtest.h" PATHS "${GTEST_ROOT_DIR}/include")
if(GTEST_INCLUDE_DIR-NOTFOUND)
    message(FATAL_ERROR "Cannot find headers of GoogleTest.")
endif()
include_directories(${GTEST_INCLUDE_DIR})

find_library(GTEST_LIB "gtest" PATHS "${GTEST_ROOT_DIR}/lib")
if(GTEST_LIB-NOTFOUND)
    message(FATAL_ERROR "Cannot find library of GoogleTest.")
endif()

add_executable(test_color_transfer test_color_transfer.cpp)

include_directories(${CMAKE_SOURCE_DIR}/source/include)
target_link_libraries(test_color_transfer ${PROJECT_NAME})
