#include <ese/flow/condition-variable.hxx>

namespace ese
{
    namespace flow
    {
        template <typename TSignal>
        TSignal ConditionVariable<TSignal>::wait(std::unique_lock<std::mutex>& lock)
        {
            WaitingThreadData data;
            auto id = std::this_thread::get_id();

            {
                std::guard_lock<std::mutex> lock(map_mutex);
                thread_map.insert(std::make_pair(id, &data));
            }

            auto predicate = [] () -> bool
            {
                return data.notified;
            };

            this->native_condition_variable.wait(lock, std::move(predicate));

            {
                std::guard_lock<std::mutex> lock(map_mutex);
                thread_map.erase(id);
            }

            return data.signal;
        }

        template <typename TSignal>
        TSignal ConditionVariable<TSignal>::notify(ThreadId id, SignalType signal) noexcept
        {
            std::guard_lock<std::mutex> lock(map_mutex);
            auto i = thread_map.find(id);

            if (i == thread_map.end())
                return;

            i.second->signaled = notified;
            i.second->signal = signal;
            native_condition_variable.notify_all();
        }


        template <typename TSignal>
        TSignal ConditionVariable<TSignal>::notify_all(SignalType signal) noexcept
        {
            std::guard_lock<std::mutex> lock(map_mutex);

            for (auto i = thread_map.begin(), end = thread.end(); i != end; ++i)
            {
                i.second->signaled = notified;
                i.second->signal = signal;
            }

            native_condition_variable.notify_all();
        }

        template <typename TSignal>
        ConditionVariable<TSignal>::WaitingThreadData::_WaitingThreadData_():
            notified(false)
        {

        }
    }
}