#ifndef STRAITX_LINUX_DISPLAY_HPP
#define STRATIX_LINUX_DISPLAY_HPP

struct _XDisplay;

namespace StraitX{
namespace Linux{

class Display{
private:
    ::_XDisplay *mHandle;
public:
    Display();

    int Open();
    int Close();

    ::_XDisplay *Handle();

    static Display &Instance();
};

};// namespace Linux::
};// namespace StraitX::

#endif