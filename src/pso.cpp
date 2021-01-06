#include "../include/pso.h"

#include <stdio.h>
#include <mpi.h>

void runPso(int dimensions, int processRank, int numberOfProcesses)
{
    bool stop = false;
    int iteration = 1;
    while (!stop)
    {
        // Compute new positions (S)
        double *localBestPosition = new double[dimensions];
        localBestPosition[0] = processRank;
        for (int i = 1; i < dimensions; i++)
            localBestPosition[i] = 0;
        // Compute new positions (E)

        // Receive local best positions (S)
        double *receivedBestPositions = NULL;
        if (processRank == ROOT)
            receivedBestPositions = new double[numberOfProcesses * dimensions];

        MPI_Gather((void *)localBestPosition, dimensions, MPI_DOUBLE,
                   (void *)receivedBestPositions, dimensions, MPI_DOUBLE,
                   ROOT, MPI_COMM_WORLD);
        // Receive local best positions (E)

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
        if (iteration == 3)
            stop = true;

        iteration++;

        // Clean just to allocate again in next iteration
        delete localBestPosition, receivedBestPositions, computedGlobalBestPosition;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    iteration--;
    if (processRank == ROOT)
        printf("[%d] Found solution after %d iterations\n", processRank, iteration);
}