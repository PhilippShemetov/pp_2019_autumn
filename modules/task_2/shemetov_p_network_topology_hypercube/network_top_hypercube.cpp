#include <mpi.h>
#include "./network_top_hypercube.h"

MPI_Comm getHypercube(int weidth,int height) {
    int sizeProc;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
}