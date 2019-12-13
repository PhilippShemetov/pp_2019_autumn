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
        vec[i] = generation() % 100;
    }
    return vec;
}

bool isSortedVec(const std::vector<int>& vec) {
    for (int i = 0; i < vec.size() - 2; i++)
        if (vec[i] > vec[i + 1])
            return false;
    return true;
}

//void swap(int* i, int* j) {
//    int temp = *i;
//    *i = *j;
//    *j = temp;
//}

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

void quickSortWithMPI(std::vector<int> vec) {
    int sizeVector = vec.size();
    if (sizeVector < 1) {
        throw "ErrorLength";
    }
    printf("check MPI");
    int rankProc, sizeProc;
    double start, end, finish;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

    if (rankProc == 0) {
        start = MPI_Wtime();
    }

    int sizeVectorNew = sizeVector;
    if (sizeVector % sizeProc != 0) {
        int offset = sizeProc - (sizeVector % sizeProc);
        sizeVectorNew += offset;
        for (int i = 0; i < offset; i++)
            vec.push_back(0);
    }
    int blockData = sizeVectorNew / sizeProc;
    std::vector<int> localVec(blockData);
   
    MPI_Scatter(&vec[0], blockData, MPI_INT, &localVec[0],
        blockData, MPI_INT, 0, MPI_COMM_WORLD);
    
    for (int i = 0; i < sizeVectorNew; i++)
    {
        std::cout << " " << vec[i];
    }
    for (int i = 0; i < blockData; i++)
    {
        std::cout << " " << localVec[i];
    }
    quickSortWithoutMPI(localVec, 0, blockData - 1);
    std::cout << std::endl;
    for (int i = 0; i < blockData; i++)
    {
        std::cout << " " << localVec[i];
    }

    MPI_Gather(&localVec[0], blockData, MPI_INT, &vec[0], blockData, MPI_INT, 0, MPI_COMM_WORLD);
    std::cout << std::endl;
    for (int i = 0; i < sizeVectorNew; i++)
    {
        std::cout << " " << vec[i];
    }

    if (rankProc == 0) {
        end = MPI_Wtime();
        finish = end - start;
        printf("Time taken: %f\n", sizeVectorNew, finish);
    }
}