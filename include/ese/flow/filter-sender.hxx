
#ifndef ESE_FLOW_FILTERSENDER_HXX
#define ESE_FLOW_FILTERSENDER_HXX

#include <ese/flow/filter.hxx>
#include <ese/flow/sender.hxx>

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
        class FilterSender: public Sender<TIn>
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
            typedef Sender<TOut> SenderType;

            /**
             * \brief Construct a FilterSender object, that interacts with a specified filer and sender.
             * \param filter The the filter that filters sent elements.
             * \param sender The sender that will receive the forwarded elements.
             * */
            FilterSender(FilterType* filter, SenderType* sender) noexcept;

            /**
             * \brief Empty implementation.
             * */
            virtual ~FilterSender() noexcept;

            /**
             * \brief Send the element.
             * \param element The element to send.
             *
             * It forwards the filtered element to the specified sender.
             * */
            void send(TIn&& element) override;

            /**
             * \brief Send the element.
             * \param element The element to send.
             *
             * It forwards the filtered element to the specified sender.
             * */
            void send(const TIn& element) override;

        private:
            /**
             * \brief The the filter that filters sent elements.
             * */
            FilterType* filter;

            /**
             * \brief The sender that will receive the forwarded elements.
             * */
            SenderType* sender;
        };
    }
}

#include "template/filter-sender.txx"

#endif
