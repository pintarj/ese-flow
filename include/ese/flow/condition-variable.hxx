
#ifndef ESE_FLOW_CONDITIONVARIABLE_HXX
#define ESE_FLOW_CONDITIONVARIABLE_HXX

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

namespace ese
{
    namespace flow
    {
        /**
         * \brief Implementation of the class std::condition_variable from C++ standard library with some additional
         *     features.
         *
         * In addition to the standard implementation this class can:
         *     - notify a specific thread
         *     - send user defined codes when notifying
         *
         * Note that due to this class features this implementation is much complex and slower by the standard one. Use
         * it wisely.
         * */
        template <typename TSignal>
        class ConditionVariable
        {
        public:
            /**
             * \brief The type of the signals.
             * */
            typedef TSignal SignalType;

            /**
             * \brief The type of a thread id.
             * */
            typedef std::thread::id ThreadId;

            /**
             * \brief Causes the current thread to block until the condition variable is notified.
             * \param lock An object which must be locked by the current thread.
             * \return The signal sent by the notifier.
             * */
            SignalType wait(std::unique_lock<std::mutex>& lock);

            /**
             * \brief Notify a thread by it's id, sending a signal.
             * \param id The id of the thread to notify.
             * \param signal The signal to send.
             *
             * If the thread was already signaled but that one did not woke up yet, then the signal will be overwritten
             * by the new passed signal.
             * */
            void notify(ThreadId id, SignalType signal) noexcept;

            /**
             * \brief Notify all waiting threads, sending a signal.
             * \param signal The signal to send.
             *
             * If any thread was already signaled but that one did not woke up yet, then the signal will be overwritten
             * by the new passed signal.
             * */
            void notify_all(ThreadId id, SignalType signal) noexcept;

        private:
            typedef struct _WaitingThreadData_
            {
                std::atomic_bool notified;
                SignalType signal;

                _WaitingThreadData_();
            }
            WaitingThreadData;

            /**
             * \brief The used native condition variable.
             * */
            std::condition_variable native_condition_variable;

            std::mutex map_mutex;

            std::map<ThreadId, WaitingThreadData*> thread_map;
        };
    }
}

#endif
