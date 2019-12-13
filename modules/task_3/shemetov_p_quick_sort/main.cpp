// Copyright 2019 Shemetov Philipp
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include "./quick_sort.h"
#include <vector>

TEST(Quick_sort, Test_Init_Sort) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::vector<int> array = { 54, -32, 11, 456, -70, 42, 16, 5, -2111, 2433 };
        std::vector<int> result = { -2111, -70, -32, 5, 11, 16, 42, 54, 456, 2433 };
        quickSortWithoutMPI(array, 0, array.size() - 1);
        ASSERT_EQ(array, result);
    }
}

TEST(Quick_sort, Test_Random_Sort) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::vector<int> randomVec;
        std::vector<int> array = randomGenerateVector(100);
        quickSortWithoutMPI(array, 0, array.size() - 1);
        EXPECT_TRUE(isSortedVec(array));
    }
}

TEST(Quick_sort, Test_Random_Sort_With_MPI) {
    int rank;
    double start, end, finish;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array(100);
    if (rank == 0) {
        array = randomGenerateVector(100);
        start = MPI_Wtime();
    }
    quickSortWithMPI(array);
    if (rank == 0) {
        end = MPI_Wtime();
        finish = end - start;
        printf("Time taken: %f\n", finish);
    }
    if (rank == 0)
        EXPECT_TRUE(isSortedVec(array));
}

TEST(Quick_sort, Test_Random_Sort_With_MPI_Large_Size) {
    int rank;
    double start, end, finish;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> array(10000000);
    if (rank == 0) {
        array = randomGenerateVector(10000000);
    }
    if (rank == 0) {
        start = MPI_Wtime();
    }
    quickSortWithMPI(array);
    if (rank == 0) {
        end = MPI_Wtime();
        finish = end - start;
        printf("Time taken: %f\n", finish);
    }
    if (rank == 0)
        EXPECT_TRUE(isSortedVec(array));
}




int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
