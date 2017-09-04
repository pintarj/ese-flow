
#ifndef ESE_FLOW_VERSION_HXX
#define ESE_FLOW_VERSION_HXX

#define ESE_FLOW_VERSION_MAJOR (0)
#define ESE_FLOW_VERSION_MINOR (0)
#define ESE_FLOW_VERSION_BUILD (0)

namespace ese
{
    namespace flow
    {
        void get_version(int* major, int* minor = nullptr, int* build = nullptr) noexcept;
    }
}

#endif
