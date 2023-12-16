cmake_minimum_required(VERSION 3.15)

set(CMAKE_PROJECT_NAME lab2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

project(${CMAKE_PROJECT_NAME})

find_package(MPI REQUIRED)

add_subdirectory(lib)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE utils)
