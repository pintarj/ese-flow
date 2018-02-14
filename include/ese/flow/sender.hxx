
#ifndef ESE_FLOW_SENDER_HXX
#define ESE_FLOW_SENDER_HXX

namespace ese
{
    namespace flow
    {
        /**
         * \brief Interface that sends elements of a specified type TElement.
         * \param TElement The type of the elements to send.
         *
         * The methods that have to be implemented are both send().
         */
        template<typename TElement>
        class Sender
        {
        public:
            /**
             * \brief The type of the elements to send.
             * */
            typedef TElement ElementType;

            /**
             * \brief Empty implementation.
             * */
            virtual ~Sender() noexcept;

            /**
             * \brief Send the element.
             * \param element The element to send.
             * */
            virtual void send(ElementType&& element) = 0;

            /**
             * \brief Send the element.
             * \param element The element to send.
             * */
            virtual void send(const ElementType& element) = 0;

            /**
             * \brief Send the element.
             * \param element The element to send.
             * \return Reference to this Sender.
             *
             * This method calls the equivalent send() method.
             * */
            Sender<ElementType>& operator<<(ElementType&& element);

            /**
             * \brief Send the element.
             * \param element The element to send.
             * \return Reference to this Sender.
             *
             * This method calls the equivalent send() method.
             * */
            Sender<ElementType>& operator<<(const ElementType& element);
        };

    }
}

#include "ese/flow/template/sender.txx"

#endif
