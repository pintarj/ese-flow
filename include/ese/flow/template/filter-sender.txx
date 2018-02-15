#include <ese/flow/filter-sender.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename TIn, typename TOut>
        FilterSender<TIn, TOut>::FilterSender(FilterType* filter, SenderType* sender) noexcept:
            filter(filter),
            sender(sender)
        {

        }

        template<typename TIn, typename TOut>
        FilterSender<TIn, TOut>::~FilterSender() noexcept
        {

        }

        template<typename TIn, typename TOut>
        void FilterSender<TIn, TOut>::send(TIn&& element)
        {
            sender->send(filter->filter(std::move(element)));
        }

        template<typename TIn, typename TOut>
        void FilterSender<TIn, TOut>::send(const TIn& element)
        {
            sender->send(filter->filter(element));
        }
    }
}
