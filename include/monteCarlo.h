#pragma once

#ifndef ROOT
#define ROOT 0
#endif

enum communicationType 
{
    BEST_SOLUTION,
    STOP
};

void runMonteCarlo(int dimensions, int processRank, int numberOfProcesses);