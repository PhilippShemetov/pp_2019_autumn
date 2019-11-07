#include <mpi.h>
#include "./network_top_hypercube.h"
#include<iostream>

MPI_Comm getHypercube(int numDims,int sizeNodeHyperCube) {
    if (numDims < 2 || sizeNodeHyperCube < 2) {
        throw "The hypercube should be >= 2d and size node should be >= 2d ";
    }
    int sizeProc,rankProc;
    int reorder = 1;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    int *sizeOfDim = new int[numDims];
    int* periods = new int[numDims];
    for (int i = 0; i < numDims; i++)
    {
        sizeOfDim[i] = sizeNodeHyperCube;
        periods[i] = 1;
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
        throw "The hypercube should be >= 2d and size node should be >= 2d ";
    }
    int countDis, status;
    int* sizeOfDim = new int[numDims];
    int* periods = new int[numDims];
    int* coord = new int[numDims];
    for (int i = 0; i < numDims; i++)
    {
        sizeOfDim[i] = sizeNodeHyperCube;
    }
    //Возвращаем кол-во измерений
    MPI_Cartdim_get(test_comm, &countDis);
    if (numDims != countDis) {
        return false;
    }
    //Получаем информацию декартовой топологии и проверям периодичность
    MPI_Cart_get(test_comm, numDims, sizeOfDim, periods, coord);
    for (int i = 0; i < numDims; i++)
    {
        if (periods[i] != 1) {
            return false;
        }
    }
    //Проверяем, является ли коммуникатор правильным типом топологии
    MPI_Topo_test(test_comm,&status);
    if (status != MPI_CART) {
        return false;
    }
    delete[]sizeOfDim;
    delete[]periods;
    delete[]coord;
    return true;
}

//Тестирование передача данных 
bool testHypercubeDataTransfer(MPI_Comm hypercube_comm, int numDims, int sizeNodeHyperCube) {
    if (numDims < 2 || sizeNodeHyperCube < 2) {
        throw "The hypercube should be >= 2d and size node should be >= 2d ";
    }
    int rankProc, sizeProc;
    int pivot = 1;
    int* coord = new int[numDims];
    int left, right;
    int local_data = 0, global_data = 0;
    MPI_Status status;
    MPI_Comm_rank(hypercube_comm, &rankProc);
    MPI_Comm_size(hypercube_comm, &sizeProc);
    MPI_Cart_coords(hypercube_comm, rankProc, numDims, coord);
    for (size_t i = 0; i < numDims; i++) {
        //получаем соседние процессы
        MPI_Cart_shift(hypercube_comm, i, 1, &left, &right); 
        if ((left == MPI_PROC_NULL) || (right == MPI_PROC_NULL))
            return false;
    }
    if (rankProc == 0) {
        local_data = 1;
    }
    MPI_Bcast(&local_data, 1, MPI_INT, 0, hypercube_comm);
    MPI_Reduce(&local_data,&global_data,1,MPI_INT,MPI_SUM,0,hypercube_comm);
    for (size_t i = 0; i < numDims; i++) {
        pivot *= sizeNodeHyperCube;
    }
    if (pivot != global_data) {
        return false;
    }
    return true;
}