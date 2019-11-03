#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include "./network_top_hypercube.h"

TEST(Network_Top_Hypercube,Test_On_Equale_Size_Proc_And_Size_Node_Hypercube) {
    int rankProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (rankProc == 0) {
        ASSERT_EQ(MPI_COMM_NULL, getHypercube(2));
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