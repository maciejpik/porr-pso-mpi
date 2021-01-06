#pragma once

int getLocalParticlesNumber(int processRank, int numberOfProcesses, int particlesNumber);
int getGlobalParticleId(int processRank, int numberOfProcesses, int particlesNumber, int localIndex);