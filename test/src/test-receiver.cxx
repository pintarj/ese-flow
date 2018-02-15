#include <gtest/gtest.h>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <ese/flow/receiver.hxx>

using namespace ese::flow;
using namespace std::chrono_literals;

class TestReceiver: public ese::flow::Receiver<int>
{
public:
    void send(int number)
    {
        std::unique_lock<std::mutex> lock(mutex);
        queue.push(number);
        condition_variable.notify_all();
    }

    bool try_receive_until_0(int* address, const boost::any& time) override
    {
        using time_point_high = std::chrono::time_point<std::chrono::high_resolution_clock>;
        using time_point_steady = std::chrono::time_point<std::chrono::steady_clock>;
        using time_point_system = std::chrono::time_point<std::chrono::system_clock>;

        if (time.type() == typeid(time_point_high))
            return try_receive_until_1(address, boost::any_cast<time_point_high>(time));

        if (time.type() == typeid(time_point_steady))
            return try_receive_until_1(address, boost::any_cast<time_point_steady>(time));

        if (time.type() == typeid(time_point_system))
            return try_receive_until_1(address, boost::any_cast<time_point_system>(time));

        throw std::exception();
    }

private:
    std::queue<int> queue;
    std::mutex mutex;
    std::condition_variable condition_variable;

    template<class Clock, class Duration>
    bool try_receive_until_1(int* address, const std::chrono::time_point<Clock, Duration>& time)
    {
        std::unique_lock<std::mutex> lock(mutex);

        if (queue.empty())
        {
            condition_variable.wait_until(lock, time);

            if (queue.empty())
                return false;
        }

        *address = queue.front();
        queue.pop();
        return true;
    }
};

class ReceiverTest: public testing::Test
{
protected:
    TestReceiver receiver;

public:
    ReceiverTest()
    {

    }
};

/*
 * Simply test if the TestReceiver class works as expected.
 */
TEST_F(ReceiverTest, simpleCheckReceiver)
{
    receiver.send(11);
    receiver.send(22);
    receiver.send(33);
    receiver.send(44);
    ASSERT_EQ(11, receiver.receive());
    ASSERT_EQ(22, receiver.receive());
    ASSERT_EQ(33, receiver.receive());
    ASSERT_EQ(44, receiver.receive());
}

/*
 * Testing if try_receive_for method works properly.
 */
TEST_F(ReceiverTest, tryReceiveFor)
{
    std::thread thread([this] ()
                       {
                           std::this_thread::sleep_for(40ms);
                           this->receiver.send(+1);
                       });

    using Clock = std::chrono::steady_clock;
    int number0 = -1;
    int number1 = -1;
    std::chrono::time_point<Clock> start = Clock::now();
    bool received0 = receiver.try_receive_for(&number0, 30ms);
    bool received1 = receiver.try_receive_for(&number1, 30ms);
    Clock::duration used_time = Clock::now() - start;
    thread.join();

    ASSERT_EQ(number0, -1);
    ASSERT_EQ(number1, +1);
    ASSERT_FALSE(received0);
    ASSERT_TRUE(received1);
    ASSERT_LT(used_time, 55ms);
}

/*
 * Testing if try_receive_until method works properly.
 */
TEST_F(ReceiverTest, tryReceiveUntil)
{
    std::thread thread([this] ()
                       {
                           std::this_thread::sleep_for(40ms);
                           this->receiver.send(+1);
                       });

    using Clock = std::chrono::steady_clock;
    int number0 = -1;
    int number1 = -1;
    std::chrono::time_point<Clock> start = Clock::now();
    bool received0 = receiver.try_receive_until(&number0, start + 30ms);
    bool received1 = receiver.try_receive_until(&number1, start + 60ms);
    Clock::duration used_time = Clock::now() - start;
    thread.join();

    ASSERT_EQ(number0, -1);
    ASSERT_EQ(number1, +1);
    ASSERT_FALSE(received0);
    ASSERT_TRUE(received1);
    ASSERT_LT(used_time, 55ms);
}

/*
 * Testing if try_receive method works properly.
 */
TEST_F(ReceiverTest, tryReceive)
{
    std::thread thread([this] ()
        {
            std::this_thread::sleep_for(40ms);
            this->receiver.send(+1);
        });

    using Clock = std::chrono::steady_clock;
    int number0 = -1;
    int number1 = -1;
    int number2 = -1;
    std::chrono::time_point<Clock> start = Clock::now();
    bool received0 = receiver.try_receive(&number0, false);
    Clock::duration used_time0 = Clock::now() - start;
    bool received1 = receiver.try_receive(&number1, true);
    bool received2 = receiver.try_receive(&number2, false);
    Clock::duration used_time1 = Clock::now() - start;
    thread.join();

    ASSERT_EQ(number0, -1);
    ASSERT_EQ(number1, +1);
    ASSERT_EQ(number2, -1);
    ASSERT_FALSE(received0);
    ASSERT_LT(used_time0, 10ms);
    ASSERT_TRUE(received1);
    ASSERT_FALSE(received2);
    ASSERT_LT(used_time1, 50ms);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
