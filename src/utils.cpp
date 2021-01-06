#include "../include/utils.h"

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