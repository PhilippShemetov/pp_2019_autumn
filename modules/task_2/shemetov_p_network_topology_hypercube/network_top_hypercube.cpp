#include <mpi.h>
#include "./network_top_hypercube.h"

MPI_Comm getHypercube(int numDims) {
    if (numDims < 2) {
        throw "The hypercube should be >=2d";
    }
    const int sizeNodeHyperCube = 2;
    int sizeProc;
    int reorder = 1, pivot = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    int *sizeOfDim = new int[numDims];
    int* periods = new int[numDims];
    for (int i = 0; i < numDims; i++)
    {
        sizeOfDim[i] = sizeNodeHyperCube;
    }
    for (int i = 0; i < numDims; i++)
    {
        periods[i] = 1;
    }
    for (int i = 0; i < numDims; i++)
    {
        pivot *= sizeOfDim[i];
    }
    if (sizeProc != pivot)
        return MPI_COMM_NULL;
    

    //MPI_Dims_create(sizeProc, dimension, ndims);
    MPI_Comm hypercube_comm;

    MPI_Cart_create(MPI_COMM_WORLD, numDims, sizeOfDim, periods, reorder, &hypercube_comm);
    
    return hypercube_comm;

}