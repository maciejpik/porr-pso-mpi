#include "include/utils.h"
#include "include/pso.h"
#include "include/monteCarlo.h"

#include <stdio.h>
#include <mpi.h>

#define ROOT 0

int main(int argc, char *argv[])
{
    int particlesNumber, dimensions;
    sscanf(argv[1], "%d", &particlesNumber);
    sscanf(argv[2], "%d", &dimensions);

    MPI_Init(&argc, &argv);

    int processRank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    if (processRank == ROOT)
    {
        printf("[%d] Number of processes is %d\n", processRank, numberOfProcesses);
        printf("[%d] Total %d particles with %d dimensions\n", processRank, particlesNumber,
               dimensions);
    }

    int localParticlesNumber = getLocalParticlesNumber(processRank, numberOfProcesses, particlesNumber);
    printf("[%d] I will compute %d particles\n", processRank, localParticlesNumber);

    // for(int i = 0; i < localParticlesNumber; i++)
    // {
    //     printf("[%d] My local particle %3d has global Id %3d\n", processRank, i,
    //         getGlobalParticleId(processRank, numberOfProcesses, particlesNumber, i));
    // }

    MPI_Barrier(MPI_COMM_WORLD);

    //runPso(dimensions, processRank, numberOfProcesses);
    runMonteCarlo(dimensions, processRank, numberOfProcesses);

    MPI_Finalize();

    return 0;
}