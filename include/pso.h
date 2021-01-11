#pragma once
#ifndef MPI_PSO_H
#define MPI_PSO_H
#ifndef ROOT
#define ROOT 0
#endif


#include <random>
#include "psoPraticle.h"
#include "OptimizationExercisesConfig.h"

void runPso(int dimensions, int processRank, int numberOfProcesses, int numberOfParticles, float stopCriterionValue, OptimizationExercisesConfig* config, bool doLog);



#endif