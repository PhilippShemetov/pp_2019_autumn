#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./sum_vector.h"

TEST(Vector_Sum, Test_Wrong_Length) {
std::vector<int> vec;
ASSERT_ANY_THROW(vec = getRandomVector(-20));
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