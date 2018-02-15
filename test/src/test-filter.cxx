#include <gtest/gtest.h>
#include <string>
#include <ese/flow/filter.hxx>

using namespace ese::flow;

class StringToIntFilter: public Filter<std::string, int>
{
public:
    int filter(std::string&& s) override
    {
        return filter(s);
    }

    int filter(const std::string& s) override
    {
        return std::stoi(s);
    }
};

class InvertIntFilter: public Filter<int, int>
{
public:
    int filter(int&& i) override
    {
        return filter(i);
    }

    int filter(const int& i) override
    {
        return -i;
    }
};

class FilterTest: public testing::Test
{
public:
    FilterTest()
    {

    }

protected:
    StringToIntFilter filter;
    InvertIntFilter invert;
};

/*
 * Just test if the filter method works well.
 */
TEST_F(FilterTest, simpleFiltering)
{
    int r0 = filter.filter("123");
    int r1 = filter.filter("456");
    int r2 = filter.filter("789");

    ASSERT_EQ(r0, 123);
    ASSERT_EQ(r1, 456);
    ASSERT_EQ(r2, 789);
}

/*
 * Test if the filter operator (bitwise or) works well.
 */
TEST_F(FilterTest, filterOperator)
{
    static const std::string v2 = "789";

    int r0 = std::string("123") | filter;
    int r1 = std::string("456") | filter;
    int r2 = v2 | filter;

    ASSERT_EQ(r0, 123);
    ASSERT_EQ(r1, 456);
    ASSERT_EQ(r2, 789);
}

/*
 * Test the filter operator chaining.
 */
TEST_F(FilterTest, filterOperatorChain)
{
    static const std::string number = "35";

    int r = number | filter | invert;

    ASSERT_EQ(r, -35);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
