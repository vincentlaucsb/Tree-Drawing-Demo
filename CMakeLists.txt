cmake_minimum_required(VERSION 3.9)
project(Tree_Drawing)

set(CMAKE_CXX_FLAGS "-lpthread -static-libgcc -static-libstdc++")
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/lib/)
add_executable(Tree_Drawing ${SOURCES} src/main.cpp src/tree.cpp)
