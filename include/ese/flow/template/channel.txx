#include <ese/flow/channel.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename TElement, typename TQueue>
        Channel<TElement, TQueue>::Channel():
            receiver(this),
            sender(this)
        {

        }

        template<typename TElement, typename TQueue>
        Receiver<TElement, TQueue>& Channel<TElement, TQueue>::get_receiver() noexcept
        {
            return receiver;
        }

        template<typename TElement, typename TQueue>
        Sender<TElement, TQueue>& Channel<TElement, TQueue>::get_sender() noexcept
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
            TElement object = std::move(front_or_top(queue));
            queue.pop();
            return object;
        }
    }
}