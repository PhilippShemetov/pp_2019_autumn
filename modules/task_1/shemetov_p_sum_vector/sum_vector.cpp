#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <mpi.h>


//Получить рандомный вектор
std::vector<int> getRandomVector(int sizeVector) {
    std::mt19937 generation;
    generation.seed(static_cast<unsigned int>(time(NULL)));
    std::vector<int> vec(sizeVector);
    for (int i = 0; i < sizeVector; i++) {
        vec[i] = generation() % 100;
    }
    return vec;
}


//Получить не рандомный вектор
std::vector<int> getVector(int sizeVector) {
    std::vector<int> vec(sizeVector);
    for (int i = 0; i < sizeVector; i++) {
        vec[i] = i;
    }
    return vec;
}

//Высчитываем сумму вектора
int getParallerSumVector(std::vector<int> &_vec, int sizeVector) {
    if (sizeVector < 1) {
        throw "ErrorLength";
    }
    int rankProc, sizeProc;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    const int blockData = sizeVector / sizeProc;
    const int blockDataRemainder = sizeVector % sizeProc;


    if (rankProc == 0) {
        for (int i = 1; i < sizeProc; i++) {
            MPI_Send(&_vec[0] + i * blockData + blockDataRemainder, blockData,
                     MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }


    std::vector<int> local_vec(blockData);
    if (rankProc == 0) {
        local_vec = std::vector<int>(_vec.begin(),
                                     _vec.begin() + blockData +
                                     blockDataRemainder);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], blockData, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 &status);


    }


    int global_sum = 0;
    int local_sum = 0;
    for (size_t i = 0; i < local_vec.size(); i++) {
        local_sum += local_vec[i];
    }
    MPI_Reduce(&local_sum, &global_sum, 1,
            MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum;
}




//Провекра
/*int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 10;
    if (rank == 0)
    {
        global_vec = getVector(count_size_vector);
    }
    int global_sum = getParallerSumVector(global_vec, count_size_vector);
    MPI_Finalize();
    return 0;
}*/