// Copyright 2019 Shemetov Philipp
#ifndef MODULES_TASK_2_SHEMETOV_P_QUICK_SORT_QUICK_SORT_H_
#define MODULES_TASK_2_SHEMETOV_P_QUICK_SORT_QUICK_SORT_H_

#include<mpi.h>
#include<vector>

bool isSortedVec(const std::vector<int>& vec);
std::vector<int> randomGenerateVector(int sizeVector);
void quickSortWithoutMPI(std::vector<int>& vec, int left, int right);
void quickSortWithMPI(std::vector<int>& vec);
void swap(std::vector<int>& arr, int i, int j);

#endif  // MODULES_TASK_2_SHEMETOV_P_QUICK_SORT_QUICK_SORT_H_