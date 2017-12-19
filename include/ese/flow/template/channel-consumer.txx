#include <ese/flow/channel-consumer.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template <typename TElement, typename TResult, typename TChannel>
        ChannelConsumer<TElement, TResult, TChannel>::
                ChannelConsumer(ChannelType* channel, const ConsumerFunctionType& consumer) noexcept:
            channel(channel),
            consumer(consumer)
        {

        }

        template <typename TElement, typename TResult, typename TChannel>
        template <typename _ResultType>
        typename std::enable_if<!std::is_same<_ResultType, void>::value, bool>::type
                ChannelConsumer<TElement, TResult, TChannel>::try_consume(ResultType* result, bool blocking)
        {
            ElementType element;

            if (!channel->get_receiver().try_receive(&element, blocking))
                return false;

            ResultType the_result = consumer(std::move(element));

            if (result != nullptr)
                *result = std::move(the_result);

            return true;
        }

        template <typename TElement, typename TResult, typename TChannel>
        template <typename _ResultType>
        typename std::enable_if<std::is_same<_ResultType, void>::value, bool>::type
                ChannelConsumer<TElement, TResult, TChannel>::try_consume(bool blocking)
        {
            ElementType element;

            if (!channel->get_receiver().try_receive(&element, blocking))
                return false;

            consumer(std::move(element));
            return true;
        }
    }
}
