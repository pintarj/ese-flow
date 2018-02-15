#include <ese/flow/channel.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename TElement, typename TQueue>
        Channel<TElement, TQueue>::Channel():
            receiver(*this),
            sender(*this)
        {

        }

        template<typename TElement, typename TQueue>
        typename Channel<TElement, TQueue>::ReceiverType& Channel<TElement, TQueue>::get_receiver() noexcept
        {
            return receiver;
        }

        template<typename TElement, typename TQueue>
        typename Channel<TElement, TQueue>::SenderType& Channel<TElement, TQueue>::get_sender() noexcept
        {
            return sender;
        }

        template<typename TElement, typename TQueue>
        void Channel<TElement, TQueue>::wake_up() noexcept
        {
            condition_variable.notify_all();
        }

        template <typename TQueue>
        static auto front_or_top(TQueue& queue) -> decltype(queue.top())
        {
            return queue.top();
        }

        template <typename TQueue>
        static auto front_or_top(TQueue& queue) -> decltype(queue.front())
        {
            return queue.front();
        }

        template<typename TElement, typename TQueue>
        TElement Channel<TElement, TQueue>::pop_from_queue() noexcept
        {
            TElement element = std::move(front_or_top(queue));
            queue.pop();
            return std::move(element);
        }

        template<typename TChannel>
        ChannelReceiver<TChannel>::ChannelReceiver(TChannel& channel) noexcept:
            channel(channel),
            channel_queue_not_empty_predicate([&channel] ()
                {
                    return !channel.queue.empty();
                })
        {

        }

        template<typename TChannel>
        bool ChannelReceiver<TChannel>::try_receive_until_0(ElementType *address, const boost::any &time)
        {
            using time_point_high = std::chrono::time_point<std::chrono::high_resolution_clock>;
            using time_point_steady = std::chrono::time_point<std::chrono::steady_clock>;
            using time_point_system = std::chrono::time_point<std::chrono::system_clock>;

            if (time.type() == typeid(time_point_high))
                return try_receive_until_1(address, boost::any_cast<time_point_high>(time));
            else if (time.type() == typeid(time_point_steady))
                return try_receive_until_1(address, boost::any_cast<time_point_steady>(time));
            else if (time.type() == typeid(time_point_system))
                return try_receive_until_1(address, boost::any_cast<time_point_system>(time));
            else
                throw std::exception(); // time_point type unknown
        }

        template<typename TChannel>
        template<class Clock, class Duration>
        bool ChannelReceiver<TChannel>::try_receive_until_1(ElementType* address, const std::chrono::time_point<Clock, Duration>& time)
        {
            std::unique_lock<std::mutex> lock(channel.mutex);

            if (!channel_queue_not_empty_predicate())
            {
                channel.condition_variable.wait_until(lock, time);

                if (!channel_queue_not_empty_predicate())
                    return false;
            }

            *address = std::move(channel.pop_from_queue());
            return true;
        }

        template<typename TChannel>
        ChannelSender<TChannel>::ChannelSender(TChannel& channel) noexcept:
            channel(channel)
        {

        }

        template<typename TChannel>
        void ChannelSender<TChannel>::send(ElementType&& element)
        {
            std::lock_guard<std::mutex> lock(channel.mutex);
            channel.queue.push(std::move(element));
            channel.condition_variable.notify_one();
        }

        template<typename TChannel>
        void ChannelSender<TChannel>::send(const ElementType& element)
        {
            std::lock_guard<std::mutex> lock(channel.mutex);
            channel.queue.push(element);
            channel.condition_variable.notify_one();
        }
    }
}