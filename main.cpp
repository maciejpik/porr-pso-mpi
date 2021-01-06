#include <stdio.h>
#include <mpi.h>

#define ROOT 0

int getLocalParticlesNumber(int processRank, int numberOfProcesses, int particlesNumber);
int getGlobalParticleId(int processRank, int numberOfProcesses, int particlesNumber, int localIndex);

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

    for(int i = 0; i < localParticlesNumber; i++)
    {
        printf("[%d] My local particle %3d has global Id %3d\n", processRank, i,
            getGlobalParticleId(processRank, numberOfProcesses, particlesNumber, i));
    }

    MPI_Finalize();

    return 0;
}

int getLocalParticlesNumber(int processRank, int numberOfProcesses, int particlesNumber)
{
    int localParticlesNumber = particlesNumber / numberOfProcesses;
    if (processRank < particlesNumber % numberOfProcesses)
        return localParticlesNumber + 1;
    else
        return localParticlesNumber;
}

int getGlobalParticleId(int processRank, int numberOfProcesses, int particlesNumber, int localIndex)
{
    int globalIdFirst = 0;
    int extendedProcesses = particlesNumber % numberOfProcesses;
    int localParticlesNumber = particlesNumber / numberOfProcesses;

    if (processRank < extendedProcesses)
        globalIdFirst = processRank * localParticlesNumber + processRank;
    else
        globalIdFirst = processRank * localParticlesNumber + extendedProcesses;

    return globalIdFirst + localIndex;
}