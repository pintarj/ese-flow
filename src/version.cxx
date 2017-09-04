#include <ese/flow/version.hxx>

namespace ese
{
    namespace flow
    {
        void get_version(int* major, int* minor, int* build) noexcept
        {
            if (major != nullptr)
                *major = ESE_FLOW_VERSION_MAJOR;

            if (minor != nullptr)
                *minor = ESE_FLOW_VERSION_MINOR;

            if (build != nullptr)
                *build = ESE_FLOW_VERSION_BUILD;
        }
    }
}
