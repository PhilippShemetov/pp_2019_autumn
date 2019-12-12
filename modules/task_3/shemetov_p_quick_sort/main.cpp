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
        std::vector<int> array = { 15, -17, 18, 8, -14, -10, 16, 12, -6, -9 };
        std::vector<int> result = { -17, -14, -10, -9, -6, 8, 12, 15, 16, 18 };
        quickSortWithoutMPI(array,0,array.size()-1);
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
