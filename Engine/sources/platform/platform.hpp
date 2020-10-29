#ifndef STRAITX_PLATFORM_HPP
#define STRAITX_PLATFORM_HPP

#include "platform/error.hpp"

namespace StraitX{

class Platform{
public:
    static Error Initialize();
    static Error Finalize();
};

}; // namespace StraitX::

#endif //STRAITX_PLATFORM_HPP