#include "../include/pso.h"
#include "../include/psoPraticle.h"

#include <stdio.h>
#include <mpi.h>

void runPso(int dimensions, int processRank, int numberOfProcesses, int numberOfParticles, float stopCriterion)
{
    bool stop = false;
    int iteration = 1;

    std::default_random_engine rand_engine;
    rand_engine.seed(time(NULL));
    for (int i = 0; i < numberOfParticles/numberOfProcesses; i++) {
        psoParticle particle;
        particle.setStartPosition(rand_engine);
        particles[i] = particle;
    }

    while (!stop) {
        // Compute new positions (S)
        //wylicz nowa pozycje dka watku
        for (psoParticle ps : particles) {
            ps.computeSpeed();
            ps.computePosition();
            ps.computeCostFunctionValue();
            if (ps.getCostFunctionValue() < localBestParticle.getCostFunctionValue()) {
                localBestParticle = ps;
            }

        }

        double *localBestPosition = new double[dimensions];
        for (int i = 0; i < dimensions; i++) {
            localBestPosition[i] = localBestParticle.getPositionVector()[i];
        }


        //wypisz tu najlepsza pozycje localna
        localBestPosition[0] = processRank;
        for (int i = 1; i < dimensions; i++)
            localBestPosition[i] = 0;
        // Compute new positions (E)

        // Receive local best positions (S)
        double *receivedBestPositions = NULL;
        if (processRank == ROOT)
            receivedBestPositions = new double[numberOfProcesses * dimensions];

        MPI_Gather((void *) localBestPosition, dimensions, MPI_DOUBLE,
                   (void *) receivedBestPositions, dimensions, MPI_DOUBLE,
                   ROOT, MPI_COMM_WORLD);
        // Receive local best positions (E)
        // znajdz wsrod odebranych pozycje!!! Find global best position



        double *computedGlobalBestPosition = new double[dimensions];
        computedGlobalBestPosition = receivedBestPositions[0];
        for (int i = 0; i < numberOfProcesses * dimensions; i++) {
        if (config->computeCostFunctionValue(receivedBestPositions[i]) <
            config->computeCostFunctionValue(computedGlobalBestPosition)) {
            computedGlobalBestPosition = receivedBestPositions[i];
        }
    }
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
        if (config->computeCostFunctionValue(computedGlobalBestPosition) < stopCriterion)
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