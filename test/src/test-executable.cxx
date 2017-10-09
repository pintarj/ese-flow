#include <gtest/gtest.h>
#include <ese/flow/executable.hxx>

using namespace ese::flow;

TEST(ExecutableTest, execution)
{
    bool success = false;
    
    Executable executable = [&success] ()
        {
            success = true;
        };
        
    executable();
    EXPECT_TRUE(success);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
