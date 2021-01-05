cmake_minimum_required(VERSION 2.8.12)

project(PORR_MPI)

find_package(MPI REQUIRED)

add_executable(PORR_MPI main.cpp)

target_link_libraries(PORR_MPI PRIVATE MPI::MPI_C)
