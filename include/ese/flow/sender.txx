#include <ese/flow/sender.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename Type>
        Sender<Type>::Sender(Channel<Type>* channel) noexcept:
            channel(channel)
        {

        }

        template<typename Type>
        Sender<Type>::~Sender() noexcept
        {

        }

        template<typename Type>
        void Sender<Type>::send(Type&& object) noexcept
        {
            std::lock_guard<std::mutex> lock(channel->mutex);
            channel->queue.push(std::move(object));
            channel->condition_variable.notify_one();
        }

        template<typename Type>
        void Sender<Type>::send(const Type& object) noexcept
        {
            std::lock_guard<std::mutex> lock(channel->mutex);
            channel->queue.push(object);
            channel->condition_variable.notify_one();
        }

        template<typename Type>
        Sender<Type>& Sender<Type>::operator<<(Type&& object) noexcept
        {
            send(std::move(object));
            return *this;
        }

        template<typename Type>
        Sender<Type>& Sender<Type>::operator<<(const Type& object) noexcept
        {
            send(object);
            return *this;
        }
    }
}
