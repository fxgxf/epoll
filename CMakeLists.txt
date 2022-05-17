cmake_minimum_required(VERSION 2.6)

project(pas CXX)

set(CXX_FLAGS -g -Wall)
set(CMAKE_CXX_COMPILER "g++")
set( CMAKE_BUILD_TYPE Debug )
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include)

FILE(GLOB INCLUDE_FILE "./include/*.cpp" "./include/*.h")

FILE(GLOB SC_FILES "./src/*.cpp" "./src/*.h")

add_executable( ${PROJECT_NAME} ${SC_FILES} ${INCLUDE_FILE})