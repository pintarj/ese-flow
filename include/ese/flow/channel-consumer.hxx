
#ifndef ESE_FLOW_CHANNELCONSUMER_HXX
#define ESE_FLOW_CHANNELCONSUMER_HXX

#include <ese/flow/channel.hxx>

namespace ese
{
    namespace flow
    {
        /**
         *  \brief A class that consumes elements from a specified channel.
         *  \param TElement The type of the elements in the channel.
         *  \param TResult The type of the consumer result.
         *  \param TChannel The type of channel that this class is consuming.
         * */
        template <typename TElement, typename TResult = void, typename TChannel = Channel<TElement>>
        class ChannelConsumer
        {
            public:
                /**
                 * \brief The type of the elements in the channel.
                 * */
                typedef TElement ElementType;

                /**
                 * \brief The type of the consumer result.
                 * */
                typedef TResult ResultType;

                /**
                 * \brief The type of channel that this class is consuming.
                 * */
                typedef TChannel ChannelType;

                /**
                 * \brief The type of the consumer function.
                 * */
                typedef std::function<ResultType(ElementType&&)> ConsumerFunctionType;

                /**
                 * \brief Construct an object that will consume elements from a specified channel and using the
                 *     specified consumer function.
                 * \param channel The channel from which the object will get the elements.
                 * \param consumer The consumer function. This will accepts elements from the channel and
                 *     consume them (generating a result).
                 * */
                ChannelConsumer(ChannelType* channel, const ConsumerFunctionType& consumer) noexcept;

                /**
                 * \brief Tries to consume an object from the channel.
                 * \param result A pointer to ResultType variable where the result will be stored (the result will be
                 *     valid only if this method will return true.
                 * \param blocking If true the method will block until an element is available on the channel (or
                 *     the "wake up" signal is thrown on channel). Otherwise, if false this method won't wait and
                 *     will return immediately if there is no available element on the channel. Default is false.
                 * \result True if there was consumed an element and the result is stored in the specified variable.
                 * */
                template <typename _ResultType = ResultType>
                typename std::enable_if<!std::is_same<_ResultType, void>::value, bool>::type
                        try_consume(ResultType* result, bool blocking = false);

                /**
                 * \brief Tries to consume an object from the channel.
                 * \param blocking If true the method will block until an element is available on the channel (or
                 *     the "wake up" signal is thrown on channel). Otherwise, if false this method won't wait and
                 *     will return immediately if there is no available element on the channel. Default is false.
                 * \result True if there was consumed an element.
                 * */
                template <typename _ResultType = ResultType>
                typename std::enable_if<std::is_same<_ResultType, void>::value, bool>::type
                        try_consume(bool blocking = false);

            private:
                /**
                 * \brief The channel specified at object creation.
                 * */
                ChannelType* channel;

                /**
                 * \brief The consumer specified at object creation.
                 * */
                ConsumerFunctionType consumer;
        };
    }
}

#include "template/channel-consumer.txx"

#endif
