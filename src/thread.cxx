#include <ese/flow/thread.hxx>

namespace ese
{
    namespace flow
    {
        static std::atomic_int running_native_threads(0);

        Thread::Thread(Executable&& executable):
            status(Status::NOT_STARTED)
        {
            Executable native_body = [this, executable = std::move(executable)] ()
                {
                    ++running_native_threads;
                    this->status = Status::RUNNING;
                    executable();
                    this->status = Status::FINISHED;
                    --running_native_threads;
                };

            native_thread = new std::thread(std::move(native_body));
        }

        Thread::~Thread()
        {
            join();
        }

        Thread::Status Thread::get_status() const noexcept
        {
            return status;
        }

        bool Thread::is_running() const noexcept
        {
            return status == Status::RUNNING;
        }

        bool Thread::is_joined() const noexcept
        {
            return status == Status::JOINED;
        }

        void Thread::join()
        {
            std::lock_guard<std::mutex> lock(joining_mutex);

            if (status == Status::JOINED)
                return;

            native_thread->join();
            status = Status::JOINED;
            delete native_thread;
        }

        int Thread::get_native_running_count() noexcept
        {
            return running_native_threads;
        }
    }
}
