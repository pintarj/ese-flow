
#ifndef ESE_FLOW_CHANNEL_HXX
#define ESE_FLOW_CHANNEL_HXX

namespace ese
{
    namespace flow
    {
        template<typename Type>
        class Channel;
    }
}

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <ese/flow/receiver.hxx>
#include <ese/flow/sender.hxx>

namespace ese
{
    namespace flow
    {
        /**
         * \brief Used to safely share data among threads.
         * \param Type The type of objects to share.
         *
         * To send objects in channel use the channel's Sender object and to receive data from the channel use the
         * channel's Receiver object. \n
         * The objects that are sent, but not received yet are stored in a queue. \n
         * All operation (even those of receiver and sender) are thread-safe.
         * */
        template <typename Type>
        class Channel
        {
            friend Receiver<Type>;
            friend Sender<Type>;

            private:
                /**
                 * \brief The channel's queue.
                 * */
                std::queue<Type> queue;

                /**
                 * \bried Mutex used to synchronize access to channel's queue.
                 * */
                std::mutex mutex;

                /**
                 * \brief Condition variable used to signal when the channel's queue is no more empty.
                 * */
                std::condition_variable condition_variable;

                /**
                 * \brief The channel's Receiver object.
                 * */
                Receiver<Type> receiver;

                /**
                 * \brief The channel's Sender object.
                 * */
                Sender<Type> sender;

                /**
                 * \brief Pops the front object from the channel's queue.
                 * \return The popped object.
                 * */
                Type pop_from_queue() noexcept;

            public:
                /**
                 * \brief Construct a Channel object.
                 * \sa get_receiver()
                 * */
                Channel();

                /**
                 * \brief Get the channel's receiver.
                 * \return The receiver.
                 * */
                Receiver<Type>& get_receiver() noexcept;

                /**
                 * \brief Get the channel's sender.
                 * \return The sender.
                 * */
                Sender<Type>& get_sender() noexcept;
        };
    }
}

#include "channel.txx"

#endif
