#include <mpi.h>
#include "./network_top_hypercube.h"
#include<iostream>

MPI_Comm getHypercube(int numDims,int sizeNodeHyperCube) {
    if (numDims < 2 || sizeNodeHyperCube < 2) {
        throw "The hypercube should be >= 2d and should size node >= 2d ";
    }
    int sizeProc,rankProc;
    int reorder = 1, pivot = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    int *sizeOfDim = new int[numDims];
    int* periods = new int[numDims];
    for (int i = 0; i < numDims; i++)
    {
        sizeOfDim[i] = sizeNodeHyperCube;
        periods[i] = 1;
        pivot *= sizeOfDim[i];
    }
  
    MPI_Comm hypercube_comm;

    MPI_Cart_create(MPI_COMM_WORLD, numDims, sizeOfDim, periods, reorder, &hypercube_comm);

    delete[]sizeOfDim;
    delete[]periods;
    return hypercube_comm;

}

//Проверка коммуникатора на тополгию гиперкуба
bool thisIsHypercube(MPI_Comm test_comm,int numDims, int sizeNodeHyperCube) {
    if (numDims < 2 || sizeNodeHyperCube < 2) {
        throw "The hypercube should be >= 2d and should size node >= 2d ";
    }
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
    delete[]sizeOfDim;
    delete[]periods;
    delete[]coord;
    return true;
}

bool testHypercubeDataTransfer(MPI_Comm hypercube_comm, int numDims, int sizeNodeHyperCube) {
    int rankProc, sizeProc;
    int rank_source, rank_dest, dataCount;
    MPI_Status status;
    MPI_Comm_rank(hypercube_comm, &rankProc);
    MPI_Comm_size(hypercube_comm, &sizeProc);
    int* coord = new int[numDims];
    MPI_Cart_coords(hypercube_comm, rankProc, numDims, coord);
    std::cout << " Rank " << rankProc << " coordinates are "
        << coord[0] << coord[1] << coord[2] << std::endl;
    
    for (size_t i = 0; i < numDims; i++) {
        MPI_Cart_shift(hypercube_comm, i, -1, &rank_source, &rank_dest);
        if ((rank_source == MPI_PROC_NULL) || (rank_dest == MPI_PROC_NULL))
            dataCount = 1;
        std::cout << rank_source << "/ " << rank_dest << std::endl;
    }
    return true;
}