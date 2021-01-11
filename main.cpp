#include "include/Utils.h"
#include "include/Settings.h"
#include "include/Mc.h"

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    Settings* settings = new Settings(argc, argv);
    
    settings->setVerbose(true);
    settings->setLogger(true);

    if (settings->getProcessRank() == settings->getRoot())
    {
        printf("[%d] Number of processes is %d\n", settings->getProcessRank(), settings->getNumberOfProcesses());
        printf("[%d] Total %d particles with %d dimensions\n", settings->getProcessRank(), settings->getParticlesNumber(),
               settings->getDimensions());
    }

    Mc monteCarloSolver(settings);
    monteCarloSolver.solve();

    MPI_Finalize();

    return 0;
}