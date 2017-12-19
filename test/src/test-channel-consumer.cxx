#include <chrono>
#include <thread>
#include <utility>
#include <gtest/gtest.h>
#include <ese/flow/channel-consumer.hxx>

#define THE_NUMBER (42)

using namespace std::chrono_literals;
using namespace ese::flow;

class ChannelConsumerTest: public testing::Test
{
    protected:
        Channel<int> channel;

    public:
        ChannelConsumerTest()
        {

        }
};

/*
 * Checking if the try_consume() method is working well if the blocking parameter is set to true.
 */
TEST_F(ChannelConsumerTest, tryConsumeBlocking)
{
    ChannelConsumer<int, int> consumer(&channel, [] (int&& in) -> int
        {
            return in;
        });

    std::thread producer([this] ()
        {
            std::this_thread::sleep_for(30ms);
            this->channel.get_sender() << THE_NUMBER;
        });

    int result = 0;
    bool consumed = consumer.try_consume(&result, true);
    producer.join();

    ASSERT_TRUE(consumed);
    ASSERT_EQ(result, THE_NUMBER);
}

/*
 * Checking the void as result type.
 */
TEST_F(ChannelConsumerTest, voidAsResultType)
{
    int result = 0;

    ChannelConsumer<int> consumer(&channel, [&result] (int&& in) -> void
        {
            result = in;
        });

    this->channel.get_sender() << THE_NUMBER;
    bool consumed = consumer.try_consume(true);

    ASSERT_TRUE(consumed);
    ASSERT_EQ(result, THE_NUMBER);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
