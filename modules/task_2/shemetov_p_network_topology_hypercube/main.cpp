#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include "./network_top_hypercube.h"

TEST(Network_Top_Hypercube, Test_Wrong_Size_Dimension_Of_Hypercube) {
    
    ASSERT_ANY_THROW(getHypercube(-1));

}

//TEST(Network_Top_Hypercube,Test_On_Equale_Size_Proc_And_Size_Node_Hypercube) {
//    MPI_Comm actual_comm = getHypercube(3);
//    
//    int rankProc;
//    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
//    if (rankProc == 0) {
//        EXPECT_NE(MPI_COMM_NULL, actual_comm);
//    }
//}

TEST(Network_Top_Hypercube, Test_This_Hypercube_Has_Not_Right_Dimension) {
    int rankProc, sizeProc;

    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (sizeProc == 4) {
        MPI_Comm actual_comm = getHypercube(2, 2);

        if (rankProc == 0) {
            ASSERT_FALSE(thisIsHypercube(actual_comm, 3, 2));
        }
    }
}

TEST(Network_Top_Hypercube, Test_This_Hypercube_Has_Right_Arguments) {
    int rankProc, sizeProc;

    MPI_Comm_size(MPI_COMM_WORLD, &sizeProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    if (sizeProc == 4) {
        MPI_Comm actual_comm = getHypercube(2, 2);
        if (rankProc == 0) {
            ASSERT_TRUE(thisIsHypercube(actual_comm, 2, 2));
        }
    }
}

//TEST(Network_Top_Hypercube, Test_Can_Create_Hypercube_Comm) {
//    MPI_Comm actual_comm = getHypercube(2);
//    int rankProc;
//    
//    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
//
//    if (rankProc == 0) {
//        EXPECT_TRUE()
//    }
//
//}












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