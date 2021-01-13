#include "../include/Settings.h"
#include "../include/Utils.h"
#include "../include/OptimizationTask_1.h"
#include "../include/OptimizationTask_2.h"

#include <mpi.h>
#include <stdio.h>
#include <time.h>

Settings::Settings(int argc, char* argv[])
{
    root = 0;

    if (argc == 3)
    {
        sscanf(argv[1], "%d", &particlesNumber);
        sscanf(argv[2], "%d", &dimensions);
    }
    else
    {
        particlesNumber = 100;
        dimensions = 2;
    }
    
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    computeLocalParticlesNumber();

    initializationConstraints = {-40, 40};
    solutionConstraints = {-40, 40};

    randomEngine.seed((processRank + 1) * time(NULL));

    task = new OptimizationTask_2();

    mc = {2, .001};
    stopCriterion = 0.01;
    verbose = true;
    logger = false;
}

void Settings::computeLocalParticlesNumber()
{
    localParticlesNumber = particlesNumber / numberOfProcesses;
    if (processRank < particlesNumber % numberOfProcesses)
        localParticlesNumber++;
}