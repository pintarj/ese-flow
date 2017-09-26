#include <ese/flow/receiver.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename Type>
        Receiver<Type>::Receiver(Channel<Type>* channel) noexcept:
            channel(channel),
            channel_queue_not_empty_predicate([channel] ()
                {
                    return !channel->queue.empty();
                })
        {

        }

        template<typename Type>
        Receiver<Type>::~Receiver() noexcept
        {

        }

        template<typename Type>
        Type Receiver<Type>::receive() noexcept
        {
            std::unique_lock<std::mutex> lock(channel->mutex);
            channel->condition_variable.wait(lock, channel_queue_not_empty_predicate);
            return std::move(channel->pop_from_queue());
        }

        template<typename Type>
        bool Receiver<Type>::try_receive(Type* address, bool blocking) noexcept
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

        template<typename Type>
        template<class Clock, class Duration>
        bool Receiver<Type>::try_receive_until(Type* address, const std::chrono::time_point<Clock, Duration>& time)
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

        template<typename Type>
        template<class Rep, class Period>
        bool Receiver<Type>::try_receive_for(Type* address, const std::chrono::duration<Rep, Period>& time)
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
    }
}