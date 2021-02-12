cmake_minimum_required(VERSION 3.10)
PROJECT(openlibc)

# setup version
set(TSNDEMO_MAJOR_VERSION 0)
set(TSNDEMO_MINOR_VERSION 0)
set(TSNDEMO_MICRO_VERSION 1)
set(TSNDEMO_VERSION ${TSNDEMO_MAJOR_VERSION}.${TSNDEMO_MINOR_VERSION}.${TSNDEMO_MICRO_VERSION})

# set default build typ if not specified by user
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE release)
endif()
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)


# include custom Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/inc)

SET(SRC_LIST
        src/util.c
        src/dynamic-string.c
        src/svec.c
        src/hash.c
        src/hmap.c
        src/smap.c
        src/simap.c
        src/sset.c
        src/vector.c
        src/queue.c
        )

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

add_library(${PROJECT_NAME}_static STATIC ${SRC_LIST})

SET_TARGET_PROPERTIES (${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
