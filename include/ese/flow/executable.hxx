
#ifndef ESE_FLOW_EXECUTABLE_HXX
#define ESE_FLOW_EXECUTABLE_HXX

#include <functional>

namespace ese
{
    namespace flow
    {
        /**
         * \brief An executable is function wrapper. That function has no arguments and returns nothing.
         * */
        typedef std::function<void()> Executable;
    }
}

#endif
