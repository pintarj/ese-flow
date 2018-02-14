
#ifndef ESE_FLOW_CHANNEL_HXX
#define ESE_FLOW_CHANNEL_HXX

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
        template<typename TChannel>
        class ChannelReceiver;

        template<typename TChannel>
        class ChannelSender;

        /**
         * \brief Used to safely share elements among threads.
         * \param TElement The type of elements to share.
         * \param TQueue The queue type used to store sent elements that waits to be received. The specified type have
         *     to implement at least pop(), front() (or top()), and push() methods (std::queue and std::priority_queue
         *     are both suitable as this template parameter).
         *
         * To send elements in channel use the channel's Sender object and to receive data from the channel use the
         * channel's Receiver object. \n
         * The elements that are sent, but not received yet are stored in a queue of template type TQueue. \n
         * All operation (even those of receiver and sender) are thread-safe. \n
         * */
        template <typename TElement, typename TQueue = std::queue<TElement>>
        class Channel
        {
        public:
            /**
             *  brief The type of elements to share.
             * */
            typedef TElement ElementType;

            /**
             *  brief The type of queue where the sent (but not received yet) elements are stored.
             * */
            typedef TQueue QueueType;

            /**
             * \brief The type of the Receiver that interacts with this Channel.
             * */
            typedef ChannelReceiver<Channel<TElement, TQueue>> ReceiverType;

            /**
             * \brief The type of the Sender that interacts with this Channel.
             * */
            typedef ChannelSender<Channel<TElement, TQueue>> SenderType;

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
             * \brief Wakes up all the threads that are waiting to receive an element via the Receiver object
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

        /**
         * \brief Receives elements from a Channel.
         * \param TChannel The type of Channel from which it receives elements.
         */
        template<typename TChannel>
        class ChannelReceiver: public Receiver<typename TChannel::ElementType>
        {
        public:
            /**
             *  brief The type of Channel from which it receives elements.
             * */
            typedef TChannel ChannelType;

            /**
             *  brief The type of receiving elements.
             * */
            typedef typename TChannel::ElementType ElementType;

        protected:
            /**
             * \brief Tries to receive an element until a time point.
             * \param address The pointer to the address where the received element have to be moved.
             * \param time The time_point to wait until (using boost::any time for accept different time_points).
             * \return True if the element was received (and moved to the address passed as argument), false otherwise.
             * \sa try_receive_until_1()
             *
             * The object will be moved to the address passed as argument and the method will return true. \n
             * Otherwise, if there is no object to receive (until the specified time point), nothing will
             * be moved to the passed address and the method will return false. \n
             * */
            bool try_receive_until_0(ElementType* address, const boost::any& time) override;

        private:
            /**
             * \brief Returns true if the channel's queue is not empty.
             * */
            std::function<bool()> channel_queue_not_empty_predicate;

            /**
             * \brief The channel from which it receives elements.
             * */
            ChannelType& channel;

            /**
             * \brief Construct the receiver, that receives elements from a specified channel.
             * \param channel The channel from which it receives elements.
             * */
            ChannelReceiver(ChannelType& channel) noexcept;

            /**
             * \brief Tries to receive an element until a time point.
             * \param address The pointer to the address where the received element have to be moved.
             * \param time The time_point to wait until.
             * \return True if the element was received (and moved to the address passed as argument), false otherwise.
             *
             * The object will be moved to the address passed as argument and the method will return true. \n
             * Otherwise, if there is no object to receive (until the specified time point), nothing will
             * be moved to the passed address and the method will return false. \n
             * */
            template<class Clock, class Duration>
            bool try_receive_until_1(int* address, const std::chrono::time_point<Clock, Duration>& time);

            friend ChannelType;
        };

        /**
         * \brief Send elements into a Channel object.
         * \param TChannel The type of Channel in which sends elements.
         * */
        template<typename TChannel>
        class ChannelSender: public Sender<typename TChannel::ElementType>
        {
        public:
            /**
             * \brief The type of Channel in which sends elements.
             * */
            typedef TChannel ChannelType;

            /**
             * \brief The type of receiving elements.
             * */
            typedef typename TChannel::ElementType ElementType;

            /**
             * \brief Send the element in the channel.
             * \param element The element to send.
             * */
            void send(ElementType&& element) override;

            /**
             * \brief Send the element in the channel.
             * \param element The element to send.
             * */
            void send(const ElementType& element) override;

        private:
            /**
             * \brief The channel in which it sends elements.
             * */
            ChannelType& channel;

            /**
             * \brief Construct the sender, that sends elements in a specified channel.
             * \param channel The channel from in which sends elements.
             * */
            ChannelSender(ChannelType& channel) noexcept;

            friend ChannelType;
        };
    }
}

#include "ese/flow/template/channel.txx"

#endif
