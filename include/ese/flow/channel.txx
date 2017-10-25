#include <ese/flow/channel.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename Type, typename Queue>
        Channel<Type, Queue>::Channel():
            receiver(this),
            sender(this)
        {

        }

        template<typename Type, typename Queue>
        Receiver<Type, Queue>& Channel<Type, Queue>::get_receiver() noexcept
        {
            return receiver;
        }

        template<typename Type, typename Queue>
        Sender<Type, Queue>& Channel<Type, Queue>::get_sender() noexcept
        {
            return sender;
        }

        template<typename Type, typename Queue>
        void Channel<Type, Queue>::wake_up() noexcept
        {
            condition_variable.notify_all();
        }

        template<typename Type, typename Queue>
        Type Channel<Type, Queue>::pop_from_queue() noexcept
        {
            Type object = std::move(queue.front());
            queue.pop();
            return object;
        }
    }
}