#include <ese/flow/filter-receiver.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename TIn, typename TOut>
        FilterReceiver<TIn, TOut>::FilterReceiver(FilterType* filter, ReceiverType* receiver) noexcept:
            filter(filter),
            receiver(receiver)
        {

        }

        template<typename TIn, typename TOut>
        FilterReceiver<TIn, TOut>::~FilterReceiver() noexcept
        {

        }

        template<typename TIn, typename TOut>
        bool FilterReceiver<TIn, TOut>::try_receive_until_0(TOut *address, const boost::any &time)
        {
            TIn in;

            if (!receiver->try_receive_until_0(&in, time))
                return false;

            *address = filter->filter(std::move(in));
            return true;
        }
    }
}
