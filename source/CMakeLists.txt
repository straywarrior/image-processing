cmake_minimum_required (VERSION 2.8)

if (WITH_OpenCV)
    include_directories("${OpenCV_INCLUDE_DIRS}")
endif (WITH_OpenCV)

include_directories("include")

set(header_list include/color_transfer.hpp
                )
set(source_list color_transfer.cpp)

source_group("Headers" FILES ${header_list})
source_group("Source Files" FILES ${source_list})
add_library(${PROJECT_NAME} STATIC ${header_list} ${source_list})

if (WITH_OpenCV)
    target_link_libraries(${PROJECT_NAME} "${OpenCV_LIBS}")
    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          INTERFACE_LINK_LIBRARIES "${OpenCV_LIBS}")
    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          INTERFACE_INCLUDE_DIRECTORIES "${OpenCV_INCLUDE_DIRS}")
endif (WITH_OpenCV)