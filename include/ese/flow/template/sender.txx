#include <ese/flow/sender.hxx>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename TElement>
        Sender<TElement>::~Sender() noexcept
        {

        }

        template<typename TElement>
        Sender<TElement>& Sender<TElement>::operator<<(ElementType&& element)
        {
            send(std::move(element));
            return *this;
        }

        template<typename TElement>
        Sender<TElement>& Sender<TElement>::operator<<(const ElementType& element)
        {
            send(element);
            return *this;
        }
    }
}