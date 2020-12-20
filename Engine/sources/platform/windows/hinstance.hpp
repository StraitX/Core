#ifndef STRAITX_WINDOWS_INSTANCE_HPP
#define STRAITX_WINDOWS_INSTANCE_HPP

#include <windows.h>
#include "platform/result.hpp"


namespace StraitX {
namespace Windows{

class HInstance {
private:
    HINSTANCE mInstanceHandle;
public:
    HInstance();

    Result Initialize();

    HINSTANCE &Handle();

    static HInstance& Instance();
};

};// namespace Windows::
};// namespace StraitX::

#endif