#include <ese/flow/executor.hxx>

namespace ese
{
    namespace flow
    {
        class DefaultExecutor: public Executor
        {
            public:
                void execute(Executable&& executable) override
                {
                    executable();
                }

                void execute(const Executable& executable) override
                {
                    executable();
                }
        };

        Executor& Executor::get_default()
        {
            static DefaultExecutor executor;
            return executor;
        }
    }
}