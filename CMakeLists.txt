project("Pascal-Compiler")
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories($(CMAKE_CURRENT_SOURCE_DIR)/source)

file(GLOB SOURCES "source/*.cpp")
file(GLOB HEADERS "source/*.h")


add_executable(pascal-compiler source/main.cpp ${HEADERS} ${SOURCES})