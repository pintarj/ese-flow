
#ifndef ESE_FLOW_RECEIVER_HXX
#define ESE_FLOW_RECEIVER_HXX

namespace ese
{
    namespace flow
    {
        template<typename Type>
        class Receiver;
    }
}

#include <chrono>
#include <functional>
#include <ese/flow/channel.hxx>

namespace ese
{
    namespace flow
    {
        /**
         * \brief Receives objects from a Channel object.
         * \param Type The type of objects to receive.
         */
        template<typename Type>
        class Receiver
        {
            friend Channel<Type>;

            private:
                /**
                 * \brief The channel from which it receives the objects.
                 * */
                Channel<Type>* channel;

                /**
                 * \brief Returns true if the channel's queue is not empty.
                 * */
                std::function<bool()> channel_queue_not_empty_predicate;

                /**
                 * \brief Create a Receiver object.
                 * \param channel The reference to the channel from which it receives the objects.
                 * */
                Receiver(Channel<Type>* channel) noexcept;

            public:
                /**
                 * \brief Destroys the object.
                 * */
                virtual ~Receiver() noexcept;

                /**
                 * \brief Receive an object from the channel.
                 * \return The object.
                 * \sa try_receive()
                 *
                 * The method will wait until there is an object to receive.
                 * */
                Type receive() noexcept;

                /**
                 * \brief Tries to receive an object from the channel.
                 * \param object The pointer to the address where the received object have to be moved.
                 * \return True if the object was received (and moved to the address passed as argument), false
                 *     otherwise.
                 * \sa receive()
                 * \sa try_receive_for()
                 *
                 * The object will be moved to the address passed as argument and the method will return true.
                 * Otherwise, if there is no object to receive, nothing will be moved to the passed address and the
                 * method will return false.
                 * */
                bool try_receive(Type* address) noexcept;

                /**
                 * \brief Tries to receive an object from the channel for an amount of time.
                 * \param object The pointer to the address where the received object have to be moved.
                 * \param time The amount of time to wait.
                 * \return True if the object was received (and moved to the address passed as argument), false
                 *     otherwise.
                 * \sa receive()
                 * \sa try_receive()
                 *
                 * The object will be moved to the address passed as argument and the method will return true.
                 * Otherwise, if there is no object to receive (for the entire specified amount of time), nothing will
                 * be moved to the passed address and the method will return false.
                 * */
                template<class Rep, class Period>
                bool try_receive_for(Type* address, const std::chrono::duration<Rep, Period>& time);
        };

    }
}

#include <ese/flow/receiver.txx>

#endif
