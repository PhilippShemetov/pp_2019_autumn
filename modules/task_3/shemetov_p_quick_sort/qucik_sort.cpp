// Copyright 2019 Shemetov Philipp
#include <mpi.h>
#include "../../../modules/task_3/shemetov_p_quick_sort/quick_sort.h"

#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <cmath>

void randomGenerateVector(std::vector<int>& vec, int sizeArr) {
    std::mt19937 generation;
    generation.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < sizeArr; i++)
    {
        vec[i] = generation() % 100;
    }

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
