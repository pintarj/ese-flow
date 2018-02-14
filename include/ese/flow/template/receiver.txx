#include <ese/flow/receiver.hxx>
#include <chrono>
#include <utility>

namespace ese
{
    namespace flow
    {
        template<typename TElement>
        Receiver<TElement>::~Receiver() noexcept
        {

        }

        template<typename TElement>
        TElement Receiver<TElement>::receive()
        {
            TElement element;
            while (!try_receive(&element, true));
            return std::move(element);
        }

        template<typename TElement>
        bool Receiver<TElement>::try_receive(TElement* address, bool blocking)
        {
            using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
            return try_receive_until(address, blocking ? time_point::max() : time_point::min());
        }

        template<typename TElement>
        template<class Clock, class Duration>
        bool Receiver<TElement>::try_receive_until(TElement* address, const std::chrono::time_point<Clock, Duration>& time)
        {
            return try_receive_until_0(address, std::chrono::time_point_cast<typename Clock::duration>(time));
        }

        template<typename TElement>
        template<class Rep, class Period>
        bool Receiver<TElement>::try_receive_for(TElement* address, const std::chrono::duration<Rep, Period>& duration)
        {
            return try_receive_until(address, std::chrono::high_resolution_clock::now() + duration);
        }
    }
}