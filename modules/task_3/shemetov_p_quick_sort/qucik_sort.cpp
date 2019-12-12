// Copyright 2019 Shemetov Philipp
#include <mpi.h>
#include "../../../modules/task_3/shemetov_p_quick_sort/quick_sort.h"
#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>

void swap(std::vector<int>& arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}


bool isSortedVec(const std::vector<int>& vec) {
    for (int i = 0; i < vec.size() - 2; i++)
        if (vec[i] > vec[i + 1])
            return false;
    return true;
}

std::vector<int> randomGenerateVector(int sizeVector) {
    if (sizeVector < 1) {
        throw "ErrorLength";
    }
    std::mt19937 generation;
    generation.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(sizeVector);
    for (int i = 0; i < sizeVector; i++)
    {
        vec[i] = generation() % 100;
    }
    return vec;
}

void quickSortWithoutMPI(std::vector<int>& vec, int left, int right) {
    int l = left, r = right;
    int pivot = vec[(left + right) / 2];
    int temp;
    do {
        while (vec[l] < pivot)
            l++;
        while (vec[r] > pivot)
            r--;
        if (l <= r) {
            temp = vec[l];
            vec[l] = vec[r];
            vec[r] = temp;
            l++;
            r--;
        }

    } while (l <= r);

    if (l < right) {
        quickSortWithoutMPI(vec, l, right);
    }
    if (left < r) {
        quickSortWithoutMPI(vec, left, r);
    }

    
}

void quickSortWithMPI(std::vector<int>& vec, int sizeVector) {
    printf("check");
    const int sizeVector = vec.size();
    if (sizeVector < 1) {
        throw "ErrorLength";
    }

    int rankProc, sizeProc;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    const int blockData = sizeVector / sizeProc;
    const int blockDataRemainder = sizeVector % sizeProc;
    printf("check1");
    std::vector<int> localVector(blockData);
    printf("check2");
    MPI_Scatter(&vec, blockData, MPI_INT, &localVector, blockData, MPI_INT, 0, MPI_COMM_WORLD);
    quickSortWithoutMPI(localVector, 0, blockData - 1);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Rank %d: \n ", rankProc);
    for (int i = 0; i < blockData; i++)
        printf("|%d| ", localVector[i]);
    printf("\n");

}