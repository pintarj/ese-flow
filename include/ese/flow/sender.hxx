
#ifndef ESE_FLOW_SENDER_HXX
#define ESE_FLOW_SENDER_HXX

namespace ese
{
    namespace flow
    {
        template<typename Type>
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
         * */
        template<typename Type>
        class Sender
        {
            friend Channel<Type>;

            private:
                /**
                 * \brief The channel in which will send the objects.
                 * */
                Channel<Type>* channel;

                /**
                 * \brief Create a Receiver object.
                 * \param channel The reference to the channel in which will send the objects.
                 * */
                Sender(Channel<Type>* channel) noexcept;

            public:
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
                Sender<Type>& operator<<(Type&& object) noexcept;

                /**
                 * \brief Send the object in the channel.
                 * \param object The object to send.
                 * \return Reference to this Sender.
                 * */
                Sender<Type>& operator<<(const Type& object) noexcept;
        };
    }
}

#include <ese/flow/sender.txx>

#endif
