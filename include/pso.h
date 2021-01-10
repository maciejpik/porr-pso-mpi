#pragma once

#ifndef ROOT
#define ROOT 0
#endif

#include <vector>
#include <random>
#include "psoPraticle.h"
#include "OptimizationExercisesConfig.h"

void runPso(int dimensions, int processRank, int numberOfProcesses, int numberOfParticles);
std::vector<psoParticle> particles;
psoParticle localBestParticle;
OptimizationExercisesConfig* config;