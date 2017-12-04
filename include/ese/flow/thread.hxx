
#ifndef ESE_FLOW_THREAD_HXX
#define ESE_FLOW_THREAD_HXX

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

namespace ese
{
    namespace flow
    {
        /**
         * \brief A thread class.
         *
         * The created Thread object will execute the specified executable on a system thread. \n
         * Every thread should be joined, via Thread::join() method. \n
         * This class is something like a std::thread class wrapper (it adds some useful methods). \n
         * All operations on objects if this class are thread-safe. \n
         * */
        class Thread
        {
            public:
                /**
                 * \brief Enumeration of possible Thread object status.
                 *
                 * When the Thread object is created it is in NOT_STARTED status, then when the OS will start the
                 * thread, the thread itself will set the status to RUNNING. When the executable passed to the thread
                 * will finish it's execution: the status will be set to FINISHED (by the thread itself). Finally, when
                 * the thread is joined, via the Thread::join() method, the status is set to JOINED.
                 * */
                enum Status
                {
                    NOT_STARTED,
                    RUNNING,
                    FINISHED,
                    JOINED
                };

            private:

                /**
                 * \brief The address of the native (C++) thread object.
                 * */
                std::thread* native_thread;

                /**
                 * \brief The status of this object.
                 * */
                std::atomic<Status> status;

                /**
                 * \brief Mutex used to synchronize thread joining.
                 * */
                std::mutex joining_mutex;

            public:
                /**
                 * \brief Creates a Thread object running the specified executable.
                 * \param executable The executable (have to implement operator()) to run on the thread.
                 * */
                template <typename TExecutable>
                Thread(TExecutable&& executable);

                /**
                 * \brief Destroys this object.
                 * \sa join()
                 *
                 * If the thread was not joined yet: it will be joined now (via Thread::join() method).
                 * */
                virtual ~Thread();

                /**
                 * \brief Return the current thread status.
                 * \return The thread status.
                 * */
                Status get_status() const noexcept;

                /**
                 * \brief Tells if the thread is running (status == RUNNING).
                 * \return True if it's running, false otherwise.
                 * */
                bool is_running() const noexcept;

                /**
                 * \brief Tells if the thread is joined (status == JOINED).
                 * \return True if it's joined, false otherwise.
                 * */
                bool is_joined() const noexcept;

                /**
                 * \brief The function returns when the thread execution has completed.
                 *
                 * Note that every thread should always me joined.
                 * */
                void join();

                /**
                 * \brief Return the current number of running native (C++) threads, created by this class.
                 * \return The number of running native (C++) threads.
                 *
                 * NOTE: The actual number of running threads may differ if some threads where created directly using
                 * the std::thread class (or any other method for thread creation).
                 * */
                static int get_native_running_count() noexcept;
        };
    }
}

#include "template/thread.txx"

#endif
