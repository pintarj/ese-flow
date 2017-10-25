
#ifndef ESE_FLOW_SENDER_HXX
#define ESE_FLOW_SENDER_HXX

namespace ese
{
    namespace flow
    {
        template<typename Type, typename Queue>
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
         * \param Type The type of objects to send.
         * \param Queue The queue type used to store sent objects that waits to be received. The specified type have to
         *     implement at least pop(), front() and push() methods (std::queue does).
         * */
        template<typename Type, typename Queue>
        class Sender
        {
            public:
                /**
                 * \brief The type of the Channel that this Sender interacts with.
                 * */
                typedef Channel<Type, Queue> ChannelType;

                /**
                 * \brief Destroys the object.
                 * */
                virtual ~Sender() noexcept;

                /**
                 * \brief Send the object in the channel.
                 * \param object The object to send.
                 * */
                void send(Type&& object) noexcept;

                /**
                 * \brief Send the object in the channel.
                 * \param object The object to send.
                 * */
                void send(const Type& object) noexcept;

                /**
                 * \brief Send the object in the channel.
                 * \param object The object to send.
                 * \return Reference to this Sender.
                 * */
                Sender<Type, Queue>& operator<<(Type&& object) noexcept;

                /**
                 * \brief Send the object in the channel.
                 * \param object The object to send.
                 * \return Reference to this Sender.
                 * */
                Sender<Type, Queue>& operator<<(const Type& object) noexcept;

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

#include <ese/flow/sender.txx>

#endif
