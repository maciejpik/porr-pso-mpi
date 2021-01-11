#include "../include/pso.h"
#include "../include/psoPraticle.h"
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <mpi.h>
#include <vector>
#include <float.h>
#include <string.h>
#include <chrono>

void runPso(int dimensions, int processRank, int numberOfProcesses, int numberOfParticles, float stopCriterionValue, OptimizationExercisesConfig* config, bool doLog) {

    auto tStart = std::chrono::high_resolution_clock::now();

    bool stop = false;
    int iteration = 1;
    std::vector<psoParticle *> particles;
    std::default_random_engine rand_engine;
    psoParticle *localBestParticle;
    particles.resize(numberOfParticles / numberOfProcesses);
    rand_engine.seed(time(NULL));
    for (int i = 0; i < numberOfParticles / numberOfProcesses; i++) {
        std::string idPart = std::to_string(processRank) + std::to_string(i);
        psoParticle *particle = new psoParticle(dimensions, config, idPart);
        particle->setStartPosition(rand_engine);
        particle->setStartSpeed(rand_engine);
        particle->computeCostFunctionValue();
        printf("[%d] current cost %f, start\n", processRank, particle->getCostFunctionValue());
        particles[i] = particle;
    }
    FILE *logFile;
    char tempFilename[100], filename[100];
    if (doLog) {
        sprintf(tempFilename, "temp_logFile_%d.txt", processRank);
        logFile = fopen(tempFilename, "w");
    }

    localBestParticle = particles[0];
    double *computedGlobalBestPosition = new double[dimensions];
    //computedGlobalBestPosition = receivedBestPositions[0];
    double globalBestPosCost = DBL_MAX;
    while (!stop) {

        // Compute new positions (S)
        //printf("%d_%d\n", iteration, processRank);
        for (psoParticle *ps : particles) {
            ps->computePosition(&rand_engine);
            ps->computeCostFunctionValue();
            if (ps->getCostFunctionValue() < localBestParticle->getCostFunctionValue()) {
                localBestParticle = ps;
            }
            if (doLog && dimensions >= 2) {
                std::vector<double> tempPosition = ps->getPositionVector();
                fprintf(logFile, "%d_%s_%lf_%lf_%lf\n", iteration, ps->getId().c_str(),
                        tempPosition[0], tempPosition[1], ps->getCostFunctionValue());
            }

            //printf("[%d] current cost %f, iteration %d\n", processRank, ps->getCostFunctionValue(), iteration);
        }

        double *localBestPosition = new double[dimensions];
        for (int i = 0; i < dimensions; i++) {
            localBestPosition[i] = localBestParticle->getPositionVector()[i];
        }
        // Compute new positions (E)*/

        // Receive local best positions (S)
        double *receivedBestPositions = NULL;
        if (processRank == ROOT)
            receivedBestPositions = new double[numberOfProcesses * dimensions];

        MPI_Gather((void *) localBestPosition, dimensions, MPI_DOUBLE,
                   (void *) receivedBestPositions, dimensions, MPI_DOUBLE,
                   ROOT, MPI_COMM_WORLD);
        // Receive local best positions (E)
        // znajdz wsrod odebranych pozycje!!! Find global best position




        if (processRank == ROOT) {
            for (int i = 0; i < numberOfProcesses * dimensions; i = i + dimensions) {
                std::vector<double> positionVectors(receivedBestPositions + i, receivedBestPositions + i + dimensions);

                double cCost;

                if ((cCost = config->computeCostFunctionValue(positionVectors)) <
                    globalBestPosCost) {
                    globalBestPosCost = cCost;
                    std::copy_n(receivedBestPositions + i, dimensions, computedGlobalBestPosition);
                }
                // printf("[%d] current cost %f, iteration %d\n", processRank, cCost, iteration);
            }
            // Compute stop criterion
            if (globalBestPosCost < stopCriterionValue)
                stop = true;
            //printf("[%d] current global best pos cost %f, iteration %d\n", processRank, globalBestPosCost, iteration);
            for (int i = 0; i < dimensions; i++) {
            }
        }

        // Broadcast global best position
        MPI_Bcast((void *) computedGlobalBestPosition, dimensions, MPI_DOUBLE, ROOT,
                  MPI_COMM_WORLD);
        MPI_Bcast((void *) &stop, 1, MPI_CXX_BOOL, ROOT,
                  MPI_COMM_WORLD);
        for (psoParticle *ps : particles) {
            ps->globalBestPosition = computedGlobalBestPosition;

        }

        // Update best position
        //printf("[%d] New best position [0] = %lf\n", processRank, computedGlobalBestPosition[0]);



        iteration++;

        // Clean just to allocate again in next iteration
        delete localBestPosition, receivedBestPositions;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    iteration--;
    if (processRank == ROOT) {
    auto tEnd = std::chrono::high_resolution_clock::now();
    long long int duration = std::chrono::duration_cast<std::chrono::microseconds>(tEnd - tStart).count();

        printf("[%d] Solution %lf (%.6lf s)\n", processRank, globalBestPosCost,
                duration / 1000000.);
    }

    if(doLog)
    {
        fclose(logFile);
        sprintf(filename, "particlesLog_p%d_%d_%d_academic_pso_%d_a_MPI.txt", processRank, iteration, dimensions,
                 config->taskNumber);
        rename(tempFilename, filename);
    }
}