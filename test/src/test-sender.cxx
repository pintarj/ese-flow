#include <gtest/gtest.h>
#include <queue>
#include <ese/flow/sender.hxx>

using namespace ese::flow;

class TestSender: public Sender<std::string>
{
public:
    std::queue<std::string> copied;
    std::queue<std::string> moved;

    void send(std::string&& s) override
    {
        moved.push(std::move(s));
    }

    void send(const std::string& s) override
    {
        copied.push(s);
    }
};

class SenderTest: public testing::Test
{
protected:
    TestSender sender;

public:
    SenderTest()
    {

    }
};

/*
 * Tests if the "move" send() method works well.
 */
TEST_F(SenderTest, moveSend)
{
    sender.send("janez");
    sender.send("pintar");

    ASSERT_EQ(sender.moved.size(), 2);
    ASSERT_EQ(sender.copied.size(), 0);
}

/*
 * Tests if the "move" send operator method works well.
 */
TEST_F(SenderTest, moveSendOp)
{
    sender << "janez";
    sender << "pintar";

    ASSERT_EQ(sender.moved.size(), 2);
    ASSERT_EQ(sender.copied.size(), 0);
}

/*
 * Tests if the "copy" send() method works well.
 */
TEST_F(SenderTest, copySend)
{
    std::string janez = "janez";
    std::string pintar = "pintar";

    sender.send(janez);
    sender.send(pintar);

    ASSERT_EQ(sender.moved.size(), 0);
    ASSERT_EQ(sender.copied.size(), 2);
}

/*
 * Tests if the "copy" send operator method works well.
 */
TEST_F(SenderTest, copySendOp)
{
    std::string janez = "janez";
    std::string pintar = "pintar";

    sender << janez;
    sender << pintar;

    ASSERT_EQ(sender.moved.size(), 0);
    ASSERT_EQ(sender.copied.size(), 2);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
