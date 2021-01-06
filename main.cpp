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

    // for(int i = 0; i < localParticlesNumber; i++)
    // {
    //     printf("[%d] My local particle %3d has global Id %3d\n", processRank, i,
    //         getGlobalParticleId(processRank, numberOfProcesses, particlesNumber, i));
    // }

    MPI_Barrier(MPI_COMM_WORLD);

    bool stop = false;
    int iteration = 1;
    while (!stop)
    {
        // Compute new posistions (S)
        double *localBestPosition = new double[dimensions];
        localBestPosition[0] = processRank;
        for (int i = 1; i < dimensions; i++)
            localBestPosition[i] = 0;
        // Compute new postisions (E)

        // Receive local best postisions (S)
        double *receivedBestPositions = NULL;
        if (processRank == ROOT)
            receivedBestPositions = new double[numberOfProcesses * dimensions];

        MPI_Gather((void *)localBestPosition, dimensions, MPI_DOUBLE,
                   (void *)receivedBestPositions, dimensions, MPI_DOUBLE,
                   ROOT, MPI_COMM_WORLD);
        // Receive local best postisions (E)

        // Find global best position
        double *computedGlobalBestPosition = new double[dimensions];
        if (processRank == ROOT)
        {
            for (int i = 0; i < dimensions; i++)
                computedGlobalBestPosition[i] = -1;
        }

        // Broadcast global best position
        MPI_Bcast((void *)computedGlobalBestPosition, dimensions, MPI_DOUBLE, ROOT,
                  MPI_COMM_WORLD);

        // Update best position
        //printf("[%d] New best position [0] = %lf\n", processRank, computedGlobalBestPosition[0]);

        // Compute stop criterion
        if(iteration == 3)
            stop = true;

        iteration++;

        // Clean just to allocate again in next iteration
        delete localBestPosition, receivedBestPositions, computedGlobalBestPosition;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    iteration--;
    if(processRank == ROOT)
        printf("[%d] Found solution after %d iterations\n", processRank, iteration);

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