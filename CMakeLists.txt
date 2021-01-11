cmake_minimum_required(VERSION 2.8.12)

project(PORR_MPI)

find_package(MPI REQUIRED)

add_executable(PORR_MPI main.cpp
    include/Settings.h src/Settings.cpp
    include/Utils.h src/Utils.cpp
    include/Particle.h src/Particle.cpp
    include/OptimizationTask.h src/OptimizationTask.cpp
    include/OptimizationTask_1.h src/OptimizationTask_1.cpp
    include/OptimizationTask_2.h src/OptimizationTask_2.cpp
    include/McParticle.h src/McParticle.cpp
    include/Mc.h src/Mc.cpp)

target_link_libraries(PORR_MPI PRIVATE MPI::MPI_C)