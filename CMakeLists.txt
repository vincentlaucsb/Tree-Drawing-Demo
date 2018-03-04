cmake_minimum_required(VERSION 3.9)
project(Tree_Drawing)

set(CMAKE_CXX_FLAGS "-lpthread -static-libgcc -static-libstdc++")
set(CMAKE_CXX_STANDARD 11)
set(SOURCES src/tree.cpp src/tree_draw.cpp)

include_directories(${CMAKE_SOURCE_DIR}/lib/)
include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/tests/)
add_executable(Tree_Drawing ${SOURCES} src/main.cpp )
add_executable(Nary_Tree_Drawing ${SOURCES} src/nary_tree.cpp src/main.cpp )
add_executable(Tree_Drawing_Test ${SOURCES} tests/tree_tests.cpp)
add_executable(Tree_Drawing_RuntimeCalc ${SOURCES} src/runtimeTest.cpp )
