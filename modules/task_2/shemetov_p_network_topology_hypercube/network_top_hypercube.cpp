#include <mpi.h>
#include "./network_top_hypercube.h"
#include<iostream>

MPI_Comm getHypercube(int numDims,int sizeNodeHyperCube) {
    if (numDims < 2 || sizeNodeHyperCube < 2) {
        throw "The hypercube should be >= 2d and should size node >= 2d ";
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
        throw "The hypercube should be >= 2d and should size node >= 2d ";
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
    int rankProc, sizeProc;
    int left, right, dataCount = 0;
    int local_data = 0;
    MPI_Status status;
    MPI_Comm_rank(hypercube_comm, &rankProc);
    MPI_Comm_size(hypercube_comm, &sizeProc);
    int* coord = new int[numDims];
    MPI_Cart_coords(hypercube_comm, rankProc, numDims, coord);
    std::cout << " Rank " << rankProc << " coordinates are "
        << coord[0] << coord[1] << coord[2] << std::endl;
    
    for (size_t i = 0; i < numDims; i++) {
        //получаем соседние процессы
        MPI_Cart_shift(hypercube_comm, i, 1, &left, &right); 
        if ((left == MPI_PROC_NULL) || (right == MPI_PROC_NULL))
            dataCount = 1;
        std::cout << "rank "<< rankProc << " : " << left << "/ " << right << std::endl;
    }
    if (dataCount) {
        return false;
    }

    /*if (rankProc == 0) {
        local_data = 1;
    }
    for (size_t i = 0; i < numDims; i++) {
        MPI_Send(&local_data,1,MPI_INT,right)
    }
    return true;*/
}