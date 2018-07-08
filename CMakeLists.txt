cmake_minimum_required(VERSION 2.8)

project(libpifacedigital++)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS} -std=gnu99 -fPIC")
set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS} -std=c++14 -fPIC")

set(LIBRARY_SOURCES
    src/PiFace.cpp
)

set(EXAMPLE_SOURCES
    src/example/main.cpp
)


set(INCLUDE_DIRS
    "${libpifacedigital++_SOURCE_DIR}"
    "${libpifacedigital++_SOURCE_DIR}/src"
    "${libpifacedigital++_SOURCE_DIR}/3rdparty/libpifacedigital/src"
)

set(LINK_DIRS
    "${libpifacedigital++_SOURCE_DIR}"
    "${libpifacedigital++_SOURCE_DIR}/3rdparty/libmcp23s17"
    "${libpifacedigital++_SOURCE_DIR}/3rdparty/libpifacedigital"
)

add_subdirectory(${libpifacedigital++_SOURCE_DIR}/3rdparty)

include_directories(${INCLUDE_DIRS})
link_directories(${LINK_DIRS})

add_library(pifacedigital++ SHARED ${LIBRARY_SOURCES})
target_link_libraries(pifacedigital++ pifacedigital mcp23s17)

add_executable(example ${EXAMPLE_SOURCES})
target_link_libraries(example pifacedigital++)

message(STATUS "Generarating ${CMAKE_SOURCE_DIR}/.clang_complete")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
file(WRITE ${CMAKE_SOURCE_DIR}/.clang_complete "")
foreach(dir ${INCLUDE_DIRS})
  file(APPEND ${CMAKE_SOURCE_DIR}/.clang_complete "-I${dir}\n")
endforeach()
string(REPLACE "'" "" CMAKE_CXX_FLAGS_SPLIT ${CMAKE_CXX_FLAGS})
string(REPLACE " " ";" CMAKE_CXX_FLAGS_SPLIT ${CMAKE_CXX_FLAGS_SPLIT})
foreach(flag ${CMAKE_CXX_FLAGS_SPLIT})
  file(APPEND ${CMAKE_SOURCE_DIR}/.clang_complete "${flag}\n")
endforeach()

# add_custom_target(documentation COMMAND doxygen WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
