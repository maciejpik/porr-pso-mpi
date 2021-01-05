#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) 
{
    MPI_Init( &argc , &argv );

    int processRank, numberOfProcesses;
    MPI_Comm_size( MPI_COMM_WORLD , &numberOfProcesses );
    MPI_Comm_rank( MPI_COMM_WORLD , &processRank );   

    printf("Process [%d]: Number of processes is %d\n", processRank, numberOfProcesses);

    MPI_Finalize();
   
   return 0;
 }