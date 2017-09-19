#include <ese/flow/channel.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename Type>
        Type Channel<Type>::pop_from_queue() noexcept
        {
            Type object = std::move(queue.front());
            queue.pop();
            return object;
        }

        template<typename Type>
        Channel<Type>::Channel():
            receiver(this),
            sender(this)
        {

        }

        template<typename Type>
        Receiver<Type>& Channel<Type>::get_receiver() noexcept
        {
            return receiver;
        }

        template<typename Type>
        Sender<Type>& Channel<Type>::get_sender() noexcept
        {
            return sender;
        }
    }
}