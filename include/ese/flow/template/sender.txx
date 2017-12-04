#include <ese/flow/sender.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename TElement, typename TQueue>
        Sender<TElement, TQueue>::~Sender() noexcept
        {

        }

        template<typename TElement, typename TQueue>
        void Sender<TElement, TQueue>::send(TElement&& object) noexcept
        {
            std::lock_guard<std::mutex> lock(channel->mutex);
            channel->queue.push(std::move(object));
            channel->condition_variable.notify_one();
        }

        template<typename TElement, typename TQueue>
        void Sender<TElement, TQueue>::send(const TElement& object) noexcept
        {
            std::lock_guard<std::mutex> lock(channel->mutex);
            channel->queue.push(object);
            channel->condition_variable.notify_one();
        }

        template<typename TElement, typename TQueue>
        Sender<TElement, TQueue>& Sender<TElement, TQueue>::operator<<(TElement&& object) noexcept
        {
            send(std::move(object));
            return *this;
        }

        template<typename TElement, typename TQueue>
        Sender<TElement, TQueue>& Sender<TElement, TQueue>::operator<<(const TElement& object) noexcept
        {
            send(object);
            return *this;
        }

        template<typename TElement, typename TQueue>
        Sender<TElement, TQueue>::Sender(ChannelType* channel) noexcept:
            channel(channel)
        {

        }
    }
}
