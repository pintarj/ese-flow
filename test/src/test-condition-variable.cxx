#include <gtest/gtest.h>
#include <ese/flow/condition-variable.hxx>

using namespace ese::flow;

class ConditionVariableTest: public testing::Test
{
public:

};

/*
 *
 */
TEST_F(ConditionVariableTest, simple)
{
    ASSERT_EQ(true, true);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
