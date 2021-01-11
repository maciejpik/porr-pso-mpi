#include "../include/Mc.h"
#include "../include/Settings.h"
#include "../include/Utils.h"

#include <mpi.h>
#include <vector>
#include <chrono>

Mc::Mc(Settings *settings)
    : settings(settings)
{
    particles.resize(settings->getLocalParticlesNumber(), nullptr);
    for(int i = 0; i < settings->getLocalParticlesNumber(); i++)
        particles[i] = new McParticle(settings, getGlobalParticleId(
            settings->getProcessRank(), settings->getNumberOfProcesses(),
            settings->getParticlesNumber(), i
        ));
}

void Mc::solve()
{
    auto tStart = std::chrono::high_resolution_clock::now();

    bool stop = false;
    int iteration = 1;

    int solutionSource;
    McParticle* localBestParticle = particles[0];
    std::vector<double> localBestPosition(settings->getDimensions());

    FILE* logFile;
    char tempFilename[100], filename[100];
    if(settings->getLogger())
    {
        sprintf(tempFilename, "temp_logFile_%d.txt", settings->getProcessRank());
        logFile = fopen(tempFilename, "w");
    }

    while (!stop)
    {
        // Compute new positions
        for(int i = 0; i < settings->getLocalParticlesNumber(); i++)
        {
            particles[i]->updatePosition();
            if(particles[i]->getCost() < localBestParticle->getCost())
                localBestParticle = particles[i];

            if(settings->getLogger() && settings->getDimensions() == 2)
            {
                std::vector<double> tempPosition = particles[i]->getPosition();
                fprintf(logFile, "%d_%d_%lf_%lf_%lf\n", iteration, particles[i]->getId(),
                    tempPosition[0], tempPosition[1], particles[i]->getCost());
            }
        }

        if(settings->getProcessRank() == settings->getRoot())
        {
            // If stop criterion is met, save it and compare other with potential solutions
            localBestPosition = localBestParticle->getPosition();
            if(localBestParticle->getCost() < settings->getStopCriterion())
            {
                stop = true;
                solutionSource = settings->getProcessRank();
            }

            MPI_Status solutionMessageStatus;
            int solutionReceived = 0;
            MPI_Iprobe(MPI_ANY_SOURCE, communicationType::BEST_SOLUTION, MPI_COMM_WORLD, &solutionReceived,
                &solutionMessageStatus);
            std::vector<bool> ranksWhoFoundSolution(settings->getNumberOfProcesses(), false);
            double *potentialSolution = new double[settings->getDimensions()];
            while(solutionReceived)
            {
                stop = true;
                ranksWhoFoundSolution[solutionMessageStatus.MPI_SOURCE] = true;
                MPI_Recv(potentialSolution, settings->getDimensions(), MPI_DOUBLE, solutionMessageStatus.MPI_SOURCE,
                    communicationType::BEST_SOLUTION, MPI_COMM_WORLD, &solutionMessageStatus);

                // If received best solution is better, save it
                std::vector<double> receivedBestPosition(potentialSolution, potentialSolution + settings->getDimensions());
                if(settings->getTask()->computeCost(receivedBestPosition)
                    < settings->getTask()->computeCost(localBestPosition))
                {
                    solutionSource = solutionMessageStatus.MPI_SOURCE;
                    localBestPosition = receivedBestPosition;
                }

                // Check for more received solutions
                solutionReceived = 0;
                MPI_Iprobe(MPI_ANY_SOURCE, communicationType::BEST_SOLUTION, MPI_COMM_WORLD, &solutionReceived,
                    &solutionMessageStatus);
            }

            // If any soltuion is found, send a proper message and join processes
            if(stop)
            {
                for(int dest = 1; dest < settings->getNumberOfProcesses(); dest++)
                {
                    if(ranksWhoFoundSolution[dest])
                        continue;
                    // Content of the message doesn't matter
                    MPI_Send(&iteration, 1, MPI_INT, dest, communicationType::STOP, MPI_COMM_WORLD);
                }
                delete potentialSolution;
            }
        }
        else
        {
            // If stop criterion is met, send the best solution to the root
            if(localBestParticle->getCost() < settings->getStopCriterion())
            {
                stop = true;
                solutionSource = settings->getProcessRank();
                localBestPosition = localBestParticle->getPosition();

                MPI_Send(&localBestPosition[0], settings->getDimensions(), MPI_DOUBLE, settings->getRoot(),
                    communicationType::BEST_SOLUTION, MPI_COMM_WORLD);
            }

            // Check if solution is already foun by other process
            MPI_Status stopMessageStatus;
            int stopReceived;
            MPI_Iprobe(settings->getRoot(), communicationType::STOP, MPI_COMM_WORLD, &stopReceived,
                &stopMessageStatus);
            if (stopReceived)
            {
                stop = true;
                int stopMessage;
                MPI_Recv(&stopMessage, 1, MPI_INT, settings->getRoot(), communicationType::STOP,
                    MPI_COMM_WORLD, &stopMessageStatus);
            }
        }
        
        iteration++;
    }
    iteration--;

    if(settings->getProcessRank() == settings->getRoot())
    {
        auto tEnd = std::chrono::high_resolution_clock::now();
	    long long int duration = std::chrono::duration_cast<std::chrono::microseconds>(tEnd - tStart).count();

        double cost = settings->getTask()->computeCost(localBestPosition);
        printf("[%d] Solution %lf from process rank %d (%.6lf s)\n", settings->getProcessRank(), cost,
            solutionSource, duration / 1000000.);
    }

    if(settings->getLogger())
    {
        fclose(logFile);
        sprintf(filename, "particlesLog_p%d_%d_%d_%lf_mc_2_a_MPI.txt", settings->getProcessRank(), iteration, settings->getDimensions(),
            settings->getStopCriterion());
        rename(tempFilename, filename);
    }
}