#include "include/utils.h"
#include "include/Settings.h"
#include "include/pso.h"
#include "include/monteCarlo.h"

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    Settings* settings = new Settings(argc, argv);

    if (settings->getProcessRank() == settings->getRoot())
    {
        printf("[%d] Number of processes is %d\n", settings->getProcessRank(), settings->getNumberOfProcesses());
        printf("[%d] Total %d particles with %d dimensions\n", settings->getProcessRank(), settings->getNumberOfProcesses(),
               settings->getDimensions());
    }

    printf("[%d] I will compute %d particles\n", settings->getProcessRank(), settings->getLocalParticlesNumber());

    for(int i = 0; i < settings->getLocalParticlesNumber(); i++)
    {
        printf("[%d] My local particle %3d has global Id %3d\n", settings->getProcessRank(), i,
            getGlobalParticleId(settings->getProcessRank(), settings->getNumberOfProcesses(),
            settings->getParticlesNumber(), i));
    }

    MPI_Barrier(MPI_COMM_WORLD);

    //runPso(dimensions, processRank, numberOfProcesses);
    runMonteCarlo(settings->getDimensions(), settings->getProcessRank(), settings->getNumberOfProcesses());

    MPI_Finalize();

    return 0;
}