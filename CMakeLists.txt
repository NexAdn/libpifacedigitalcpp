cmake_minimum_required(VERSION 2.8)

project(libpifacedigitalcpp)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS} -std=gnu99 -fPIC")
set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS} -std=c++14 -fPIC -pthread")

set(LIBRARY_SOURCES
    src/PiFace.cpp
)

set(EXAMPLE_SOURCES
    src/example/main.cpp
)


set(INCLUDE_DIRS
    "${libpifacedigitalcpp_SOURCE_DIR}"
    "${libpifacedigitalcpp_SOURCE_DIR}/src"
    "${libpifacedigitalcpp_SOURCE_DIR}/3rdparty/libpifacedigital/src"
)

set(LINK_DIRS
    "${libpifacedigitalcpp_SOURCE_DIR}"
    "${libpifacedigitalcpp_BINARY_DIR}"
    "${libpifacedigitalcpp_BINARY_DIR}/3rdparty"
)

add_subdirectory(${libpifacedigitalcpp_SOURCE_DIR}/3rdparty)

include_directories(${INCLUDE_DIRS})
link_directories(${LINK_DIRS})

add_library(pifacedigitalcpp SHARED ${LIBRARY_SOURCES})
target_link_libraries(pifacedigitalcpp pifacedigital mcp23s17)

add_executable(example ${EXAMPLE_SOURCES})
target_link_libraries(example pifacedigitalcpp)
