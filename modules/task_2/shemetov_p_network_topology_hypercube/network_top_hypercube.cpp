#include <mpi.h>
#include "./network_top_hypercube.h"

MPI_Comm getHypercube(int dimension) {
    int sizeProc;
    int sizeNodeHyperCube = 2;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    int *ndims = new int[dimension];
    for (int i = 0; i < dimension; i++)
    {
        ndims[i] = sizeNodeHyperCube;
    }
    int pivot = 1;
    for (int i = 0; i < dimension; i++)
    {
        pivot *= ndims[i];
    }
    if (sizeProc % pivot != 0)
        return MPI_COMM_NULL;

    MPI_Dims_create(sizeProc, dimension, ndims);
}