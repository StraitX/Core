#ifndef STRAITX_LINUX_DISPLAY_HPP
#define STRATIX_LINUX_DISPLAY_HPP

#include "platform/error.hpp"

struct _XDisplay;

namespace StraitX{
namespace Linux{

class Display{
private:
    ::_XDisplay *mHandle;
public:
    Display();

    Error Open();
    Error Close();

    bool IsOpened();

    ::_XDisplay *Handle();

    static Display &Instance();
};

};// namespace Linux::
};// namespace StraitX::

#endif