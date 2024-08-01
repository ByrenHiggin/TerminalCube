cmake_minimum_required(VERSION 3.28)
project(3dCube)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(3dCube main.cpp)
