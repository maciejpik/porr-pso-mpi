#include "../include/monteCarlo.h"

#include <stdio.h>
#include <mpi.h>
#include <vector>

void runMonteCarlo(int dimensions, int processRank, int numberOfProcesses)
{
    bool stop = false;
    int iteration = 1; // It is meaningless for the global view
    int solutionSource;
    while (!stop)
    {
        // Compute new positions (S)
        double *localBestPosition = new double[dimensions];
        localBestPosition[0] = processRank;
        for (int i = 1; i < dimensions; i++)
            localBestPosition[i] = 0;
        // Compute new positions (E)

        if (processRank == ROOT)
        {
            // If stop criterion is met, save it and compare other with potential solutions
            if (false)
            {
                stop = true;
                solutionSource = processRank;
            }

            MPI_Status solutionMessageStatus;
            int solutionReceived = 0;
            MPI_Iprobe(MPI_ANY_SOURCE, communicationType::BEST_SOLUTION, MPI_COMM_WORLD, &solutionReceived,
                       &solutionMessageStatus);
            std::vector<bool> ranksFoundSolution(numberOfProcesses, false);
            double *potentialSolution = new double[dimensions];
            while (solutionReceived)
            {
                stop = true;
                ranksFoundSolution[solutionMessageStatus.MPI_SOURCE] = true;
                MPI_Recv(potentialSolution, dimensions, MPI_DOUBLE, solutionMessageStatus.MPI_SOURCE,
                         communicationType::BEST_SOLUTION, MPI_COMM_WORLD, &solutionMessageStatus);
                // If potential best solution is better, save it
                if (true)
                {
                    solutionSource = solutionMessageStatus.MPI_SOURCE;
                }

                // Check for more received solutions
                solutionReceived = 0;
                MPI_Iprobe(MPI_ANY_SOURCE, communicationType::BEST_SOLUTION, MPI_COMM_WORLD,
                           &solutionReceived, &solutionMessageStatus);
            }

            // If any solution is found, send a proper message and join processes
            if (stop)
            {
                for (int dest = 1; dest < numberOfProcesses; dest++)
                {
                    if (ranksFoundSolution[dest])
                        continue;
                    // Content of the message doesn't matter
                    MPI_Send(&iteration, 1, MPI_INT, dest, communicationType::STOP, MPI_COMM_WORLD);
                }
                delete localBestPosition, potentialSolution;
                break;
            }
        }
        else
        {
            // If stop criterion is met, send the best solution to the root
            if (processRank == 1 && iteration == 1000)
            {
                MPI_Send(localBestPosition, dimensions, MPI_DOUBLE, ROOT, communicationType::BEST_SOLUTION,
                         MPI_COMM_WORLD);
                delete localBestPosition;
                break;
            }

            // Check if solution is already found
            MPI_Status stopMessageStatus;
            int stopReceived = 0;
            MPI_Iprobe(ROOT, communicationType::STOP, MPI_COMM_WORLD, &stopReceived, &stopMessageStatus);
            if (stopReceived)
            {
                int stopMessage;
                MPI_Recv(&stopMessage, 1, MPI_INT, ROOT, communicationType::STOP, MPI_COMM_WORLD,
                         &stopMessageStatus);
                delete localBestPosition;
                break;
            }
        }

        iteration++;
    }

    if (processRank == ROOT)
        printf("[%d] Found solution after %d iterations from process rank %d\n", processRank, iteration,
            solutionSource);
}