#include <ese/flow/receiver.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename TElement, typename TQueue>
        Receiver<TElement, TQueue>::~Receiver() noexcept
        {

        }

        template<typename TElement, typename TQueue>
        TElement Receiver<TElement, TQueue>::receive() noexcept
        {
            std::unique_lock<std::mutex> lock(channel->mutex);
            channel->condition_variable.wait(lock, channel_queue_not_empty_predicate);
            return std::move(channel->pop_from_queue());
        }

        template<typename TElement, typename TQueue>
        bool Receiver<TElement, TQueue>::try_receive(TElement* address, bool blocking) noexcept
        {
            std::unique_lock<std::mutex> lock(channel->mutex);

            if (!channel_queue_not_empty_predicate())
            {
                if (!blocking)
                    return false;

                channel->condition_variable.wait(lock);

                if (!channel_queue_not_empty_predicate())
                    return false;
            }

            *address = std::move(channel->pop_from_queue());
            return true;
        }

        template<typename TElement, typename TQueue>
        template<class Clock, class Duration>
        bool Receiver<TElement, TQueue>::try_receive_until(TElement* address, const std::chrono::time_point<Clock, Duration>& time)
        {
            std::unique_lock<std::mutex> lock(channel->mutex);

            if (!channel_queue_not_empty_predicate())
            {
                channel->condition_variable.wait_until(lock, time);

                if (!channel_queue_not_empty_predicate())
                    return false;
            }

            *address = std::move(channel->pop_from_queue());
            return true;
        }

        template<typename TElement, typename TQueue>
        template<class Rep, class Period>
        bool Receiver<TElement, TQueue>::try_receive_for(TElement* address, const std::chrono::duration<Rep, Period>& time)
        {
            std::unique_lock<std::mutex> lock(channel->mutex);

            if (!channel_queue_not_empty_predicate())
            {
                channel->condition_variable.wait_for(lock, time);

                if (!channel_queue_not_empty_predicate())
                    return false;
            }

            *address = std::move(channel->pop_from_queue());
            return true;
        }

        template<typename TElement, typename TQueue>
        Receiver<TElement, TQueue>::Receiver(ChannelType* channel) noexcept:
            channel(channel),
            channel_queue_not_empty_predicate([channel] ()
                {
                    return !channel->queue.empty();
                })
        {

        }
    }
}