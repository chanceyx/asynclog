cmake_minimum_required(VERSION 2.8)
project(asynclog)

set(CMAKE_VERBOSE_MAKEFILE on)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -g -std=c++14 -Wall -Wno-deprecated -Werror -Wno-unused-function")

aux_source_directory(./src SUB_SRC_LIST)
aux_source_directory(./src/util SUB_SRC_LIST_UTIL)

set(SRC_LIST
    ${SUB_SRC_LIST}
    ${SUB_SRC_LIST_UTIL}
    )

add_library(asynclog SHARED ${SRC_LIST})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
