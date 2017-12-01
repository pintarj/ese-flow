#include <ese/flow/sender.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename Type, typename Queue>
        Sender<Type, Queue>::~Sender() noexcept
        {

        }

        template<typename Type, typename Queue>
        void Sender<Type, Queue>::send(Type&& object) noexcept
        {
            std::lock_guard<std::mutex> lock(channel->mutex);
            channel->queue.push(std::move(object));
            channel->condition_variable.notify_one();
        }

        template<typename Type, typename Queue>
        void Sender<Type, Queue>::send(const Type& object) noexcept
        {
            std::lock_guard<std::mutex> lock(channel->mutex);
            channel->queue.push(object);
            channel->condition_variable.notify_one();
        }

        template<typename Type, typename Queue>
        Sender<Type, Queue>& Sender<Type, Queue>::operator<<(Type&& object) noexcept
        {
            send(std::move(object));
            return *this;
        }

        template<typename Type, typename Queue>
        Sender<Type, Queue>& Sender<Type, Queue>::operator<<(const Type& object) noexcept
        {
            send(object);
            return *this;
        }

        template<typename Type, typename Queue>
        Sender<Type, Queue>::Sender(ChannelType* channel) noexcept:
            channel(channel)
        {

        }
    }
}
