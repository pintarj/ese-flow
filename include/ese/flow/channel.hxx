
#ifndef ESE_FLOW_CHANNEL_HXX
#define ESE_FLOW_CHANNEL_HXX

namespace ese
{
    namespace flow
    {
        template<typename TElement, typename TQueue>
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
         * \param TElement The type of elements to share.
         * \param TQueue The queue type used to store sent objects that waits to be received. The specified type have to
         *     implement at least pop(), front() (or top()), and push() methods (std::queue and std::priority_queue are
         *     both suitable as this template parameter).
         *
         * To send objects in channel use the channel's Sender object and to receive data from the channel use the
         * channel's Receiver object. \n
         * The objects that are sent, but not received yet are stored in a queue of template type TQueue. \n
         * All operation (even those of receiver and sender) are thread-safe.
         * */
        template <typename TElement, typename TQueue = std::queue<TElement>>
        class Channel
        {
            public:
                /**
                 * \brief The type of the Receiver that interacts with this Channel.
                 * */
                typedef Receiver<TElement, TQueue> ReceiverType;

                /**
                 * \brief The type of the Sender that interacts with this Channel.
                 * */
                typedef Sender<TElement, TQueue> SenderType;

                /**
                 * \brief Construct a Channel object.
                 * \sa get_receiver()
                 * \sa get_sender()
                 * */
                Channel();

                /**
                 * \brief Get the channel's receiver.
                 * \return The receiver.
                 * */
                ReceiverType& get_receiver() noexcept;

                /**
                 * \brief Get the channel's sender.
                 * \return The sender.
                 * */
                SenderType& get_sender() noexcept;

                /**
                 * \brief Wakes up all the threads that are waiting to receive an object via the Receiver object
                 *     owned by this Channel object.
                 * */
                void wake_up() noexcept;

            private:
                /**
                 * \brief The channel's queue.
                 * */
                TQueue queue;

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
                ReceiverType receiver;

                /**
                 * \brief The channel's Sender object.
                 * */
                SenderType sender;

                /**
                 * \brief Pops the front object from the channel's queue.
                 * \return The popped object.
                 * */
                TElement pop_from_queue() noexcept;

            friend ReceiverType;
            friend SenderType;
        };
    }
}

#include "template/channel.txx"

#endif
