cmake_minimum_required(VERSION 2.8.12)

project(PORR_MPI)

find_package(MPI REQUIRED)

add_executable(PORR_MPI main.cpp
    include/utils.h src/utils.cpp
    include/pso.h src/pso.cpp
    include/monteCarlo.h src/monteCarlo.cpp
    include/psoPraticle.h src/psoPraticle.cpp
    include/OptimizationExercisesConfig.h src/OptimizationExercisesConfig.cpp
        src/PositionVectorOperator.cpp include/PositionVectorOperator.h
        src/ConfigEx1.cpp include/OptimizationExercisesConfig.h
        src/ConfigEx2.cpp include/ConfigEx1.h
        src/OptimizationExercisesConfig.cpp include/ConfigEx2.h

        include/ConfigEx1.h src/ConfigEx1.cpp
    include/ConfigEx2.h src/ConfigEx2.cpp
        )

target_link_libraries(PORR_MPI PRIVATE MPI::MPI_C)