
#ifndef ESE_FLOW_EXECUTOR_HXX
#define ESE_FLOW_EXECUTOR_HXX

namespace ese
{
    namespace flow
    {
        /**
         * \brief Interface that executes Executable objects.
         * \param TExecutable The executables executed by the executor. Have to implement operator().
         * */
        template <typename TExecutable>
        class Executor
        {
            public:
                /**
                 * \brief Execute an executable object.
                 * \param executable The object to execute.
                 * */
                virtual void execute(TExecutable&& executable);

                /**
                 * \brief Execute an executable object.
                 * \param executable The object to execute.
                 * */
                virtual void execute(const TExecutable& executable);
        };
    }
}

#include "template/executor.txx"

#endif
