#include <functional>
#include <gtest/gtest.h>
#include <ese/flow/executor.hxx>

using namespace ese::flow;

class ExecutorTest: public testing::Test
{
    public:
        ExecutorTest()
        {

        }
};

/*
 * Testing if the default implementation does execute the executable.
 */
TEST_F(ExecutorTest, defaultImplementation)
{
    bool success = false;
    Executor<std::function<void()>> executor;

    executor.execute([&success] ()
        {
            success = true;
        });
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
