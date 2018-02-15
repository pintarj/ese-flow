
#ifndef ESE_FLOW_FILTER_HXX
#define ESE_FLOW_FILTER_HXX

namespace ese
{
    namespace flow
    {
        /**
         * \brief Filters input elements of type TIn into output elements of type TOut.
         * \tparam TIn The type of input elements.
         * \tparam TOut The type of output elements.
         *
         * This is an interface class and both (for move and copy semantics) filter() methods have to be implemented.
         */
        template<typename TIn, typename TOut>
        class Filter
        {
        public:
            /**
             * \brief The type of input elements.
             * */
            typedef TIn InType;

            /**
             * \brief The type of output elements.
             * */
            typedef TOut OutType;

            /**
             * \brief Empty implementation.
             * */
            virtual ~Filter() noexcept;

            /**
             * \brief Filters input element into output element.
             * \param in The input element.
             * \return The output element.
             * */
            virtual TOut filter(TIn&& in) = 0;

            /**
             * \brief Filters input element into output element.
             * \param in The input element.
             * \return The output element.
             * */
            virtual TOut filter(const TIn& in) = 0;
        };

        /**
         * \brief Filters input element into output element.
         * \tparam TIn The type of input element.
         * \tparam TOut The type of output element.
         * \param in The input element.
         * \param filter The filter.
         * \return The output element.
         * */
        template<typename TIn, typename TOut>
        inline TOut operator|(TIn&& in, Filter<TIn, TOut>& filter);

        /**
         * \brief Filters input element into output element.
         * \tparam TIn The type of input element.
         * \tparam TOut The type of output element.
         * \param in The input element.
         * \param filter The filter.
         * \return The output element.
         * */
        template<typename TIn, typename TOut>
        inline TOut operator|(const TIn& in, Filter<TIn, TOut>& filter);
    }
}

#include "template/filter.txx"

#endif
