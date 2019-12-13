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

std::vector<int> randomGenerateVector(int sizeVector) {
    if (sizeVector < 1) {
        throw "ErrorLength";
    }
    std::mt19937 generation;
    generation.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(sizeVector);
    for (int i = 0; i < sizeVector; i++)
    {
        vec[i] = generation() % 100000000;
    }
    return vec;
}

bool isSortedVec(const std::vector<int>& vec) {
    for (int i = 0; i < vec.size() - 1; i++)
        if (vec[i] > vec[i + 1])
            return false;
    return true;
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

void quickSortWithMPI(std::vector<int>& vec) {
    int sizeVector = vec.size();
    if (sizeVector < 1) {
        throw "ErrorLength";
    }
    int rankProc, sizeProc;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    int blockData = sizeVector / sizeProc;
    std::vector<int> localVec(blockData);

    MPI_Scatter(&vec[0], blockData, MPI_INT, &localVec[0],
        blockData, MPI_INT, 0, MPI_COMM_WORLD);

    quickSortWithoutMPI(localVec, 0, blockData - 1);

    MPI_Gather(&localVec[0], blockData, MPI_INT, &vec[0], blockData, MPI_INT, 0, MPI_COMM_WORLD);

    if (rankProc == 0) {
        quickSortWithoutMPI(vec, 0, sizeVector - 1);
    }
}