#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include <ese/flow/channel.hxx>

#define THE_NUMBER  (42)
#define ONE         (1)

using namespace ese::flow;

class ChannelTest: public testing::Test
{
    protected:
        Channel<int> channel;
        Channel<int>::ReceiverType& receiver;
        Channel<int>::SenderType& sender;

    public:
        ChannelTest():
            receiver(channel.get_receiver()),
            sender(channel.get_sender())
        {

        }
};

/*
 * Tries to pass some values through the channel and checks if those are received in the correct order.
 */
TEST_F(ChannelTest, simpleValuePassing)
{
    sender.send(THE_NUMBER);
    sender << ONE;

    ASSERT_EQ(receiver.receive(), THE_NUMBER);
    ASSERT_EQ(receiver.receive(), ONE);
}

/*
 * Checks that the receiver does not block on empty channel using method ::try_receive().
 */
TEST_F(ChannelTest, noBlockOnEmptyChannel)
{
    bool received = receiver.try_receive(nullptr);
    ASSERT_FALSE(received);
}

/*
 * Checks that the receiver blocks until there is a value on the channel using ::try_receive() method.
 */
TEST_F(ChannelTest, blockUntilValueOnChannel)
{
    std::thread thread([&sender = channel.get_sender()] ()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            sender.send(THE_NUMBER);
        });

    int n = 0;
    bool received = receiver.try_receive(&n, true);
    thread.join();

    ASSERT_EQ(n, THE_NUMBER);
    ASSERT_TRUE(received);
}

/*
 * Checks that the receiver blocks until the channel calls ::wake_up() method.
 */
TEST_F(ChannelTest, blockUntilChannelWakesUp)
{
    auto start = std::chrono::steady_clock::now();

    std::thread thread([this] ()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            this->channel.wake_up();
        });

    bool received = receiver.try_receive(nullptr, true);
    auto end = std::chrono::steady_clock::now();
    thread.join();

    ASSERT_GT(end - start, std::chrono::milliseconds(90));
    ASSERT_FALSE(received);
}

/*
 * Checks that the receiver blocks for 100ms (at least) using the method ::try_receive_until where there is no data to
 * receive.
 */
TEST_F(ChannelTest, tryReceiveUntilNoData)
{
    auto start = std::chrono::steady_clock::now();
    bool received = receiver.try_receive_until(nullptr, start + std::chrono::milliseconds(100));
    auto end = std::chrono::steady_clock::now();

    ASSERT_GT(end - start, std::chrono::milliseconds(90));
    ASSERT_FALSE(received);
}

/*
 * Checks that the receiver does not block for more than specified time using the method ::try_receive_until where
 * there is data available.
 */
TEST_F(ChannelTest, tryReceiveUntilData)
{
    sender << THE_NUMBER;
    auto start = std::chrono::steady_clock::now();
    int n = 0;
    bool received = receiver.try_receive_until(&n, start + std::chrono::milliseconds(100));
    auto end = std::chrono::steady_clock::now();

    ASSERT_EQ(n, THE_NUMBER);
    ASSERT_LT(end - start, std::chrono::milliseconds(90));
    ASSERT_TRUE(received);
}

/*
 * Checks that the receiver, using the ::try_receive_until() method, blocks until the channel calls the ::wake_up()
 * method.
 */
TEST_F(ChannelTest, tryReceiveUntilNoDataWakeUp)
{
    auto start = std::chrono::steady_clock::now();

    std::thread thread([this] ()
        {
            this->channel.wake_up();
        });

    bool received = receiver.try_receive_until(nullptr, start + std::chrono::milliseconds(100));
    auto end = std::chrono::steady_clock::now();
    thread.join();

    ASSERT_LT(end - start, std::chrono::milliseconds(90));
    ASSERT_FALSE(received);
}

/*
 * Checks that the receiver blocks for 100ms (at least) using the method ::try_receive_for() where there is no data to
 * receive.
 */
TEST_F(ChannelTest, tryReceiveForNoData)
{
    auto start = std::chrono::steady_clock::now();
    bool received = receiver.try_receive_for(nullptr, std::chrono::milliseconds(100));
    auto end = std::chrono::steady_clock::now();

    ASSERT_GT(end - start, std::chrono::milliseconds(90));
    ASSERT_FALSE(received);
}

/*
 * Checks that the receiver does not block for more than specified time using the method ::try_receive_for() where
 * there is data available.
 */
TEST_F(ChannelTest, tryReceiveForData)
{
    sender << THE_NUMBER;
    auto start = std::chrono::steady_clock::now();
    int n = 0;
    bool received = receiver.try_receive_for(&n, std::chrono::milliseconds(100));
    auto end = std::chrono::steady_clock::now();

    ASSERT_EQ(n, THE_NUMBER);
    ASSERT_LT(end - start, std::chrono::milliseconds(90));
    ASSERT_TRUE(received);
}

/*
 * Checks that the receiver, using the ::try_receive_for() method, blocks until the channel calls the ::wake_up()
 * method.
 */
TEST_F(ChannelTest, tryReceiveForNoDataWakeUp)
{
    auto start = std::chrono::steady_clock::now();

    std::thread thread([this] ()
        {
            this->channel.wake_up();
        });

    bool received = receiver.try_receive_for(nullptr, std::chrono::milliseconds(100));
    auto end = std::chrono::steady_clock::now();
    thread.join();

    ASSERT_LT(end - start, std::chrono::milliseconds(90));
    ASSERT_FALSE(received);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
