cmake_minimum_required(VERSION 2.8.12)
project(PORR_MPI)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(PORR_MPI main.cpp)

find_package(MPI)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
target_link_libraries(PORR_MPI ${MPI_C_LIBRARIES})