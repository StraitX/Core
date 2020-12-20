#ifndef STRAITX_PLATFORM_HPP
#define STRAITX_PLATFORM_HPP

#include "platform/result.hpp"

namespace StraitX{

class Platform{
public:
    static Result Initialize();
    static Result Finalize();
};

}; // namespace StraitX::

#endif //STRAITX_PLATFORM_HPP