#include "../include/Settings.h"
#include "../include/utils.h"

#include <mpi.h>
#include <stdio.h>

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
}

void Settings::computeLocalParticlesNumber()
{
    localParticlesNumber = particlesNumber / numberOfProcesses;
    if (processRank < particlesNumber % numberOfProcesses)
        localParticlesNumber++;
}