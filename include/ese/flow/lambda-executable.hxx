
#ifndef ESE_FLOW_LAMBDAEXECUTABLE_HXX
#define ESE_FLOW_LAMBDAEXECUTABLE_HXX

#include <functional>

namespace ese
{
    namespace flow
    {
        /**
         * \brief Represent a C++ lambda function type that accepts no arguments and returns no value.
         * */
        typedef std::function<void()> LambdaExecutable;
    }
}

#endif
