#include <chrono>
#include <memory>
#include <thread>
#include <utility>
#include <gtest/gtest.h>
#include <ese/flow/thread.hxx>

using namespace std::chrono_literals;
using namespace ese::flow;

class ThreadTest: public testing::Test
{
    public:
        ThreadTest()
        {

        }
};

/*
 * Just testing if the function given to the thread is run and if the join works properly.
 */
TEST_F(ThreadTest, simpleRunAndJoin)
{
    bool success = false;

    Thread thread([&success] ()
                      {
                      success = true;
                      std::this_thread::sleep_for(100ms);
                      });

    thread.join();

    ASSERT_TRUE(success);
}

/*
 * Testing if ese::flow::Thread::get_native_running_count() returns the correct number of threads.
 */
TEST_F(ThreadTest, get_native_running_count)
{
    auto t0 = Thread::get_native_running_count();
    Thread thread1([] () { std::this_thread::sleep_for(120ms); });
    std::this_thread::sleep_for(20ms);
    auto t1 = Thread::get_native_running_count();
    std::this_thread::sleep_for(20ms);
    Thread thread2([] () { std::this_thread::sleep_for(120ms); });
    std::this_thread::sleep_for(20ms);
    auto t2 = Thread::get_native_running_count();
    std::this_thread::sleep_for(20ms);
    Thread thread3([] () { std::this_thread::sleep_for(120ms); });
    std::this_thread::sleep_for(20ms);
    auto t3 = Thread::get_native_running_count();
    std::this_thread::sleep_for(40ms);
    auto t4 = Thread::get_native_running_count();
    std::this_thread::sleep_for(40ms);
    auto t5 = Thread::get_native_running_count();
    std::this_thread::sleep_for(40ms);
    auto t6 = Thread::get_native_running_count();

    thread1.join();
    thread2.join();
    thread3.join();

    ASSERT_EQ(t0, 0);
    ASSERT_EQ(t1, 1);
    ASSERT_EQ(t2, 2);
    ASSERT_EQ(t3, 3);
    ASSERT_EQ(t4, 2);
    ASSERT_EQ(t5, 1);
    ASSERT_EQ(t6, 0);
}

/*
 * Testing the status of a thread during its life-cycle.
 */
TEST_F(ThreadTest, status)
{
    Thread thread([] ()
        {
            std::this_thread::sleep_for(40ms);
        });

    // Those asserts does big speculation, so they are skipped:
    // ASSERT_EQ(thread.get_status(), Thread::Status::NOT_STARTED);
    // ASSERT_FALSE(thread.is_running());
    // ASSERT_FALSE(thread.is_joined());

    std::this_thread::sleep_for(20ms);
    ASSERT_EQ(thread.get_status(), Thread::Status::RUNNING);
    ASSERT_TRUE(thread.is_running());
    ASSERT_FALSE(thread.is_joined());

    std::this_thread::sleep_for(40ms);
    ASSERT_EQ(thread.get_status(), Thread::Status::FINISHED);
    ASSERT_FALSE(thread.is_running());
    ASSERT_FALSE(thread.is_joined());

    thread.join();
    ASSERT_EQ(thread.get_status(), Thread::Status::JOINED);
    ASSERT_FALSE(thread.is_running());
    ASSERT_TRUE(thread.is_joined());
}

class CustomExecutable
{
    public:
        std::shared_ptr<bool> called;

        CustomExecutable() noexcept:
            called(new bool {false})
        {

        }

        void operator ()() noexcept
        {
            *called = true;
        }
};

/*
 * Testing if the thread run also a custom executable.
 */
TEST_F(ThreadTest, customExecutable)
{
    CustomExecutable executable;
    std::shared_ptr<bool> pointer = executable.called;
    Thread thread(std::move(executable));
    thread.join();
    ASSERT_TRUE(*pointer);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
