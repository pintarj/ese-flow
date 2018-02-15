#include <ese/flow/filter.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename TIn, typename TOut>
        Filter<TIn, TOut>::~Filter() noexcept
        {

        }

        template<typename TIn, typename TOut>
        inline TOut operator|(TIn&& in, Filter<TIn, TOut>& filter)
        {
            return filter.filter(std::move(in));
        }

        template<typename TIn, typename TOut>
        inline TOut operator|(const TIn& in, Filter<TIn, TOut>& filter)
        {
            return filter.filter(in);
        }
    }
}