
#ifndef ESE_FLOW_RECEIVER_HXX
#define ESE_FLOW_RECEIVER_HXX

#include <chrono>
#include <boost/any.hpp>

namespace ese
{
    namespace flow
    {
        /**
         * \brief Interface that receives elements of a specified type TElement.
         * \param TElement The type of the elements to receive.
         *
         * The only method that have to be implemented is try_receive_until_0(). Any other method at the end
         * will use this one.
         */
        template<typename TElement>
        class Receiver
        {
        public:
            /**
             * \brief The type of the elements to receive.
             * */
            typedef TElement ElementType;

            /**
             * \brief Empty implementation.
             * */
            virtual ~Receiver() noexcept;

            /**
             * \brief Receive an element.
             * \return The element.
             * \sa try_receive()
             *
             * The method will wait until there is an element to receive.
             * */
            TElement receive();

            /**
             * \brief Tries to receive an element.
             * \param address The pointer to the address where the received element have to be moved.
             * \param blocking If true, the method will block until an element is received.
             * \return True if the element was received (and moved to the address passed as argument), false otherwise.
             * \sa receive()
             * \sa try_receive_for()
             * \sa try_receive_until()
             *
             * The object will be moved to the address passed as argument and the method will return true. \n
             * Otherwise, if there is no object to receive, nothing will be moved to the passed address and the
             * method will return false. \n
             * */
            bool try_receive(TElement* address, bool blocking = false);

            /**
             * \brief Tries to receive an element until a time point.
             * \param address The pointer to the address where the received element have to be moved.
             * \param time The time point to wait until.
             * \return True if the element was received (and moved to the address passed as argument), false otherwise.
             * \sa receive()
             * \sa try_receive()
             * \sa try_receive_for()
             *
             * The object will be moved to the address passed as argument and the method will return true. \n
             * Otherwise, if there is no object to receive (until the specified time point), nothing will be moved to
             * the passed address and the method will return false. \n
             * */
            template<class Clock, class Duration>
            bool try_receive_until(TElement* address, const std::chrono::time_point<Clock, Duration>& time);

            /**
             * \brief Tries to receive an element for an amount of time.
             * \param address The pointer to the address where the received element have to be moved.
             * \param duration The amount of time to wait.
             * \return True if the element was received (and moved to the address passed as argument), false otherwise.
             * \sa receive()
             * \sa try_receive()
             * \sa try_receive_until()
             *
             * The object will be moved to the address passed as argument and the method will return true. \n
             * Otherwise, if there is no object to receive (for the entire specified amount of time), nothing will
             * be moved to the passed address and the method will return false. \n
             * */
            template<class Rep, class Period>
            bool try_receive_for(TElement* address, const std::chrono::duration<Rep, Period>& duration);

        protected:
            /**
             * \brief Tries to receive an element for an amount of time.
             * \param address The pointer to the address where the received element have to be moved.
             * \param duration The amount of time to wait (using boost::any time for accept different time_points).
             * \return True if the element was received (and moved to the address passed as argument), false otherwise.
             * \sa receive()
             * \sa try_receive()
             * \sa try_receive_until()
             * \sa try_receive_for()
             *
             * The object will be moved to the address passed as argument and the method will return true. \n
             * Otherwise, if there is no object to receive (for the entire specified amount of time), nothing will
             * be moved to the passed address and the method will return false. \n
             */
            virtual bool try_receive_until_0(TElement* address, const boost::any& duration) = 0;
        };

    }
}

#include "ese/flow/template/receiver.txx"

#endif
