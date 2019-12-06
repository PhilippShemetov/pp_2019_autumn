// Copyright 2019 Shemetov Philipp
#ifndef MODULES_TASK_2_SHEMETOV_P_QUICK_SORT_QUICK_SORT_H_
#define MODULES_TASK_2_SHEMETOV_P_QUICK_SORT_QUICK_SORT_H_

#include<mpi.h>

void randomGenerateVector(std::vector<int>& vec, int sizeArr);
void quickSortWithoutMPI(std::vector<int>& vec, int left, int right);


#endif  // MODULES_TASK_2_SHEMETOV_P_QUICK_SORT_QUICK_SORT_H_
