
#ifndef ESE_FLOW_SENDER_HXX
#define ESE_FLOW_SENDER_HXX

namespace ese
{
    namespace flow
    {
        template<typename TElement, typename TQueue>
        class Sender;
    }
}

#include <ese/flow/channel.hxx>

namespace ese
{
    namespace flow
    {
        /**
         * \brief Send objects into a Channel object.
         * \param TElement The type of elements to send.
         * \param TQueue The queue type used to store sent objects that waits to be received. The specified type have to
         *     implement at least pop(), front() (or top()), and push() methods (std::queue and std::priority_queue are
         *     both suitable as this template parameter).
         * */
        template<typename TElement, typename TQueue>
        class Sender
        {
            public:
                /**
                 * \brief The type of the Channel that this Sender interacts with.
                 * */
                typedef Channel<TElement, TQueue> ChannelType;

                /**
                 * \brief Destroys the object.
                 * */
                virtual ~Sender() noexcept;

                /**
                 * \brief Send the object in the channel.
                 * \param object The object to send.
                 * */
                void send(TElement&& object) noexcept;

                /**
                 * \brief Send the object in the channel.
                 * \param object The object to send.
                 * */
                void send(const TElement& object) noexcept;

                /**
                 * \brief Send the object in the channel.
                 * \param object The object to send.
                 * \return Reference to this Sender.
                 * */
                Sender<TElement, TQueue>& operator<<(TElement&& object) noexcept;

                /**
                 * \brief Send the object in the channel.
                 * \param object The object to send.
                 * \return Reference to this Sender.
                 * */
                Sender<TElement, TQueue>& operator<<(const TElement& object) noexcept;

            private:
                /**
                 * \brief The channel in which will send the objects.
                 * */
                ChannelType* channel;

                /**
                 * \brief Create a Receiver object.
                 * \param channel The reference to the channel in which will send the objects.
                 * */
                Sender(ChannelType* channel) noexcept;

            friend ChannelType;
        };
    }
}

#include "ese/flow/template/channel-sender.txx"

#endif
