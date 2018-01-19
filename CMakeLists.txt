cmake_minimum_required(VERSION 3.7)
project(mediefight)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/utils/objvbo.h)
add_executable(mediefight ${SOURCE_FILES})