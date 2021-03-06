#include <gtest/gtest.h>
#include <ese/flow/consumer.hxx>
#include <ese/flow/channel.hxx>
#include <ese/flow/thread.hxx>

using namespace ese::flow;
using namespace std::chrono_literals;

class TestConsumerFactory: public ConsumerFactory<int>
{
public:
    TestConsumerFactory(Receiver<int>* receiver):
        ConsumerFactory(receiver),
        sum(0)
    {

    }

    void consume_0(int&& number) override
    {
        sum += number;
    }

    int get_sum() const noexcept
    {
        return sum;
    }

private:
    int sum;
};

class ConsumerTest: public testing::Test
{
public:
    ConsumerTest():
        consumer_factory(&channel.get_receiver()),
        sender(channel.get_sender())
    {

    }

protected:
    Channel<int> channel;
    Sender<int>& sender;
    TestConsumerFactory consumer_factory;
};

/*
 * Check consuming some ints with non-blocking create_one() behaviour.
 */
TEST_F(ConsumerTest, nonBlockingCreateOne)
{
    sender << 1;
    sender << 2;
    sender << 3;

    Consumer<int> consumer = consumer_factory.create_one();
    int consumed[3];
    int consumed_sum[4];
    int partial_sum[4];

    for (int i = 0; i < 3; ++i)
    {
        partial_sum[i]  = consumer_factory.get_sum();
        consumed_sum[i] = consumer.get_consumed_count();
        consumed[i]     = consumer.consume();
    }

    partial_sum[3]  = consumer_factory.get_sum();
    consumed_sum[3] = consumer.get_consumed_count();

    ASSERT_EQ(consumed[0], 1);
    ASSERT_EQ(consumed[1], 1);
    ASSERT_EQ(consumed[2], 1);

    ASSERT_EQ(partial_sum[0], 0);
    ASSERT_EQ(partial_sum[1], 1);
    ASSERT_EQ(partial_sum[2], 3);
    ASSERT_EQ(partial_sum[3], 6);

    ASSERT_EQ(consumed_sum[0], 0);
    ASSERT_EQ(consumed_sum[1], 1);
    ASSERT_EQ(consumed_sum[2], 2);
    ASSERT_EQ(consumed_sum[3], 3);
}

/*
 * Check the Consumer move constructor.
 */
TEST_F(ConsumerTest, consumerMoveConstructor)
{
    sender << 42;

    Consumer<int> consumer0 = consumer_factory.create_one();
    Consumer<int> consumer(std::move(consumer0));

    int consumed    = consumer();
    int sum         = consumer_factory.get_sum();

    ASSERT_EQ(consumed, 1);
    ASSERT_EQ(sum, 42);
}

/*
 * Check consuming some ints with create_one_until() behaviour
 */
TEST_F(ConsumerTest, nonBlockingCreateOneUntil)
{
    using Clock = std::chrono::high_resolution_clock;
    auto now = Clock::now();

    Thread thread([now, this] ()
                  {
                      std::this_thread::sleep_until(now + 30ms);
                      this->sender << 1;
                      std::this_thread::sleep_until(now + 60ms);
                      this->sender << 2;
                      std::this_thread::sleep_until(now + 90ms);
                      this->sender << 3;
                  });

    Consumer<int> consumer0 = consumer_factory.create_one_until(now + 10ms);
    Consumer<int> consumer1 = consumer_factory.create_one_until(now + 70ms);
    Consumer<int> consumer2 = consumer_factory.create_one_until(now + 120ms);

    int consumed[6];
    int consumed_sum[6];
    int partial_sum[6];

    consumed[0] = consumer0();
    partial_sum[0] = consumer_factory.get_sum();
    consumed_sum[0] = consumer0.get_consumed_count();
    consumed[1] = consumer1();
    partial_sum[1] = consumer_factory.get_sum();
    consumed_sum[1] = consumer1.get_consumed_count();
    consumed[2] = consumer1();
    partial_sum[2] = consumer_factory.get_sum();
    consumed_sum[2] = consumer1.get_consumed_count();
    consumed[3] = consumer1();
    partial_sum[3] = consumer_factory.get_sum();
    consumed_sum[3] = consumer1.get_consumed_count();
    consumed[4] = consumer2();
    partial_sum[4] = consumer_factory.get_sum();
    consumed_sum[4] = consumer2.get_consumed_count();
    consumed[5] = consumer2();
    partial_sum[5] = consumer_factory.get_sum();
    consumed_sum[5] = consumer2.get_consumed_count();

    thread.join();

    ASSERT_EQ(consumed[0], 0);
    ASSERT_EQ(consumed[1], 1);
    ASSERT_EQ(consumed[2], 1);
    ASSERT_EQ(consumed[3], 0);
    ASSERT_EQ(consumed[4], 1);
    ASSERT_EQ(consumed[5], 0);

    ASSERT_EQ(partial_sum[0], 0);
    ASSERT_EQ(partial_sum[1], 1);
    ASSERT_EQ(partial_sum[2], 3);
    ASSERT_EQ(partial_sum[3], 3);
    ASSERT_EQ(partial_sum[4], 6);
    ASSERT_EQ(partial_sum[5], 6);

    ASSERT_EQ(consumed_sum[0], 0);
    ASSERT_EQ(consumed_sum[1], 1);
    ASSERT_EQ(consumed_sum[2], 2);
    ASSERT_EQ(consumed_sum[3], 2);
    ASSERT_EQ(consumed_sum[4], 1);
    ASSERT_EQ(consumed_sum[5], 1);
}

/*
 * Check consuming some ints with create_one_for() behaviour
 */
TEST_F(ConsumerTest, nonBlockingCreateOneFor)
{

    Thread thread([this] ()
        {
            std::this_thread::sleep_for(30ms);
            this->sender << 1;
            std::this_thread::sleep_for(30ms);
            this->sender << 2;
            std::this_thread::sleep_for(30ms);
            this->sender << 3;
        });

    Consumer<int> consumer = consumer_factory.create_one_for(20ms);

    int consumed[6];
    int consumed_sum[6];
    int partial_sum[6];

    for (int i = 0; i < 6; ++i)
    {
        consumed[i] = consumer();
        partial_sum[i] = consumer_factory.get_sum();
        consumed_sum[i] = consumer.get_consumed_count();
    }

    thread.join();

    ASSERT_EQ(consumed[0], 0);
    ASSERT_EQ(consumed[1], 1);
    ASSERT_EQ(consumed[2], 0);
    ASSERT_EQ(consumed[3], 1);
    ASSERT_EQ(consumed[4], 0);
    ASSERT_EQ(consumed[5], 1);

    ASSERT_EQ(partial_sum[0], 0);
    ASSERT_EQ(partial_sum[1], 1);
    ASSERT_EQ(partial_sum[2], 1);
    ASSERT_EQ(partial_sum[3], 3);
    ASSERT_EQ(partial_sum[4], 3);
    ASSERT_EQ(partial_sum[5], 6);

    ASSERT_EQ(consumed_sum[0], 0);
    ASSERT_EQ(consumed_sum[1], 1);
    ASSERT_EQ(consumed_sum[2], 1);
    ASSERT_EQ(consumed_sum[3], 2);
    ASSERT_EQ(consumed_sum[4], 2);
    ASSERT_EQ(consumed_sum[5], 3);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
