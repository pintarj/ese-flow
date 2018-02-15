#include <gtest/gtest.h>
#include <string>
#include <ese/flow/channel.hxx>
#include <ese/flow/filter-sender.hxx>

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

class FilterSenderTest: public testing::Test
{
public:
    FilterSenderTest():
        sender(&filter, &channel.get_sender()),
        receiver(channel.get_receiver())
    {

    }

protected:
    Channel<int> channel;
    StringToIntFilter filter;
    FilterSender<std::string, int> sender;
    Receiver<int>& receiver;

};

/*
 * Test if all works well.
 */
TEST_F(FilterSenderTest, simple)
{
    static const std::string fifty = "50";

    sender.send("44");
    sender << fifty;

    int r0 = receiver.receive();
    int r1 = receiver.receive();

    ASSERT_EQ(r0, 44);
    ASSERT_EQ(r1, 50);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
