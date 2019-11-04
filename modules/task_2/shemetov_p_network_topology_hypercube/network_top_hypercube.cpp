#include <mpi.h>
#include "./network_top_hypercube.h"
#include<iostream>

MPI_Comm getHypercube(int numDims) {
    if (numDims < 2) {
        throw "The hypercube should be >=2d";
    }
    const int sizeNodeHyperCube = 2;
    int sizeProc,rankProc;
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
    /*MPI_Cart_coords(hypercube_comm,rankProc,numDims,coord);
    std::cout << " Rank " << rankProc << " coordinates are " 
        << coord[0] << coord[1] << coord[2] << std::endl;*/


    return hypercube_comm;

}

//Проверка коммуникатора на тополгию гиперкуба
bool thisIsHypercube(MPI_Comm test_comm,int numDims) {
    const int sizeNodeHyperCube = 2;
    int countDis, status;
    int* sizeOfDim = new int[numDims];
    int* periods = new int[numDims];
    int* coord = new int[numDims];
    //Возвращаем кол-во измерений
    MPI_Cartdim_get(test_comm, &countDis);
    if (numDims != countDis) {
        return false;
    }
    //Проверяем, является ли коммуникатор правильным типом топологии
    MPI_Topo_test(test_comm,&status);
    if (status != MPI_CART) {
        return false;
    }
    for (int i = 0; i < numDims; i++)
    {
        sizeOfDim[i] = sizeNodeHyperCube;
    }
    //Получаем информацию декартовой топологии и проверям периодичность
    MPI_Cart_get(test_comm, numDims,sizeOfDim, periods, coord);
    for (int i = 0; i < numDims; i++)
    {
        if (periods[i] != 1) {
            return false;
        }
    }
    return true;
}

//bool hypercubeDataTransfer(MPI_Comm test_comm) {
//    int rankProc, sizeProc;
//}