
#ifndef ESE_FLOW_EXECUTOR_HXX
#define ESE_FLOW_EXECUTOR_HXX

#include <ese/flow/executable.hxx>

namespace ese
{
    namespace flow
    {
        /**
         * \brief Interface that executes Executable objects.
         * */
        class Executor
        {
            public:
                /**
                 * \brief Execute an Executable object.
                 * \param executable The object to execute.
                 * */
                virtual void execute(Executable&& executable) = 0;

                /**
                 * \brief Execute an Executable object.
                 * \param executable The object to execute.
                 * */
                virtual void execute(const Executable& executable) = 0;

                /**
                 * \brief Get the reference of the default Executor implementation object.
                 * \return Reference of the default executor implementation object.
                 *
                 * The default Executor implementation executes Executable object, passed as argument to the execute()
                 * method, immediately on the current thread.
                 * */
                static Executor& get_default();
        };
    }
}

#endif
