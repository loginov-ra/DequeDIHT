
#define IteratorDequeTest DISABLED_IteratorDequeTest
#include "Testing.h"

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    bool res = RUN_ALL_TESTS();

    std::cin.get();
    std::cin.get();
    return 0;
}