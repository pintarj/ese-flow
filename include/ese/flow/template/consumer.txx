#include <ese/flow/consumer.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename TElement>
        ConsumerFactory<TElement>::ConsumerFactory(ReceiverType* receiver) noexcept:
            receiver(receiver)
        {

        }

        template<typename TElement>
        ConsumerFactory<TElement>::~ConsumerFactory() noexcept
        {

        }

        template<typename TElement>
        typename ConsumerFactory<TElement>::ConsumerType ConsumerFactory<TElement>::create_one(bool blocking)
        {
            std::function<int(ConsumerType*)> behaviour = [this, blocking] (ConsumerType* consumer) -> int
                {
                    TElement element;

                    if (!this->receiver->try_receive(&element, blocking))
                        return 0;

                    this->consume_0(std::move(element));
                    ++consumer->data->consumed_count;
                    return 1;
                };

            return ConsumerType(std::move(behaviour));
        }

        template<typename TElement>
        template<class Clock, class Duration>
        typename ConsumerFactory<TElement>::ConsumerType ConsumerFactory<TElement>::create_one_until(const std::chrono::time_point<Clock, Duration> &time)
        {
            std::function<int(ConsumerType*)> behaviour = [this, time = time] (ConsumerType* consumer) -> int
                {
                    TElement element;

                    if (!this->receiver->try_receive_until(&element, time))
                        return 0;

                    this->consume_0(std::move(element));
                    ++consumer->data->consumed_count;
                    return 1;
                };

            return ConsumerType(std::move(behaviour));
        }

        template<typename TElement>
        template<class Rep, class Period>
        typename ConsumerFactory<TElement>::ConsumerType ConsumerFactory<TElement>::create_one_for(const std::chrono::duration<Rep, Period>& duration)
        {
            std::function<int(ConsumerType*)> behaviour = [this, duration = duration] (ConsumerType* consumer) -> int
                {
                    TElement element;

                    if (!this->receiver->try_receive_for(&element, duration))
                        return 0;

                    this->consume_0(std::move(element));
                    ++consumer->data->consumed_count;
                    return 1;
                };

            return ConsumerType(std::move(behaviour));
        }

        template<typename TElement>
        Consumer<TElement>::Consumer(Consumer<ElementType>&& other):
            data(std::move(other.data))
        {
            other.data = nullptr;
        }

        template<typename TElement>
        Consumer<TElement>::~Consumer()
        {
            if (data != nullptr)
                delete data;
        }

        template<typename TElement>
        int Consumer<TElement>::consume()
        {
            return data->behaviour(this);
        }

        template<typename TElement>
        int Consumer<TElement>::get_consumed_count() const noexcept
        {
            return data->consumed_count;
        }

        template<typename TElement>
        int Consumer<TElement>::operator()()
        {
            return consume();
        }

        template<typename TElement>
        void Consumer<TElement>::require_stop() noexcept
        {
            data->stop_required = true;
        }

        template<typename TElement>
        Consumer<TElement>::InnerData::_InnerData_(std::function<int(ThisType*)>&& behaviour):
            behaviour(std::move(behaviour)),
            consumed_count(0),
            stop_required(false)
        {

        }

        template<typename TElement>
        Consumer<TElement>::Consumer(std::function<int(ThisType*)>&& behaviour):
                data(new InnerData(std::move(behaviour)))
        {

        }
    }
}
