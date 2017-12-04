#include <ese/flow/thread.hxx>
#include <functional>
#include <utility>

namespace ese
{
    namespace flow
    {
        extern std::atomic_int running_native_threads;

        template <typename TExecutable>
        Thread::Thread(TExecutable&& executable):
            status(Status::NOT_STARTED)
        {
            std::function<void()> wrapper = [this, inner = std::move(executable)] () mutable
                {
                    ++running_native_threads;
                    this->status = Status::RUNNING;
                    inner();
                    this->status = Status::FINISHED;
                    --running_native_threads;
                };

            native_thread = new std::thread(std::move(wrapper));
        }
    }
}
