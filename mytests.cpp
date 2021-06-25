#include <gtest/gtest.h>
#include <memory>
#include "compiler.h"

TEST(myfunctions, compile)
{
    std::unique_ptr<Compiler> compiler;
    GTEST_ASSERT_EQ(compiler->compile("2 + 2"), 4);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}