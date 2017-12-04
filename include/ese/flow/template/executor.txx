#include <ese/flow/executor.hxx>

namespace ese
{
    namespace flow
    {
        template <typename TExecutable>
        void Executor<TExecutable>::execute(TExecutable&& executable)
        {
            executable();
        }

        template <typename TExecutable>
        void Executor<TExecutable>::execute(const TExecutable& executable)
        {
            executable();
        }
    }
}
