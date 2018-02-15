
#ifndef ESE_FLOW_FILTERRECEIVER_HXX
#define ESE_FLOW_FILTERRECEIVER_HXX

#include <ese/flow/filter.hxx>
#include <ese/flow/receiver.hxx>

namespace ese
{
    namespace flow
    {
        /**
         * \brief A Sender implementation that performs filtering on sent elements.
         * \tparam TIn The type of sent elements before filtering.
         * \tparam TOut The type of sent elements after filtering.
         *
         * This Sender implementation class operates with two other objects: the first is another Sender object (that
         * sends TOut elements), the seconds is a Filter object (that shares the same TIn and TOut). All the elements
         * of type TIn sent via this FilterSender object will be firstly filtered using the specified Filter object
         * (so the new type of those sent elements would become TOut) and finally forwarded to the specified Sender
         * object.
         */
        template<typename TIn, typename TOut>
        class FilterReceiver: public Receiver<TOut>
        {
        public:
            /**
             * \brief The type of sent elements before filtering.
             * */
            typedef TIn InType;

            /**
             * \brief The type of sent elements after filtering.
             * */
            typedef TOut OutType;

            /**
             * \brief The type of the filter that filters sent elements.
             * */
            typedef Filter<TIn, TOut> FilterType;

            /**
             * \brief The type of sender that will receive the forwarded elements.
             * */
            typedef Receiver<TIn> ReceiverType;

            /**
             * \brief Construct a FilterSender object, that interacts with a specified filer and sender.
             * \param filter The the filter that filters sent elements.
             * \param sender The sender that will receive the forwarded elements.
             * */
            FilterReceiver(FilterType* filter, ReceiverType* receiver) noexcept;

            /**
             * \brief Empty implementation.
             * */
            virtual ~FilterReceiver() noexcept;

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
             * */
            bool try_receive_until_0(TOut* address, const boost::any& time) override;

        private:
            /**
             * \brief The the filter that filters sent elements.
             * */
            FilterType* filter;

            /**
             * \brief The sender that will receive the forwarded elements.
             * */
            ReceiverType* receiver;
        };
    }
}

#include "template/filter-receiver.txx"

#endif
