
#ifndef ESE_FLOW_VERSION_HXX
#define ESE_FLOW_VERSION_HXX

#define ESE_FLOW_VERSION_MAJOR (0)
#define ESE_FLOW_VERSION_MINOR (3)
#define ESE_FLOW_VERSION_BUILD (0)

namespace ese
{
    namespace flow
    {
        /**
         * \brief Get the library version.
         * \param major An address where will be stored the major version number.
         * \param minor An address where will be stored the minor version number.
         * \param build An address where will be stored the build version number.
         * */
        void get_version(int* major, int* minor = nullptr, int* build = nullptr) noexcept;
    }
}

#endif
