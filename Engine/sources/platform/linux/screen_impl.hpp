#ifndef STRAITX_LINUX_SCREEN_IMPL_HPP
#define STRAITX_LINUX_SCREEN_IMPL_HPP

#include "platform/types.hpp"

namespace StraitX{
namespace Linux{

class WindowImpl;

class ScreenImpl{
private:
    unsigned int m_Index;
    void *m_Handle;
    Size2i m_Size;
    Size2f m_DPI;

    friend class WindowImpl;
public:
    ScreenImpl(unsigned int index, void *handle, const Size2i &size, const Size2f &dpi);

    ~ScreenImpl() = default;
    
    const Size2i &Size()const;

    const Size2f &DPI()const;
};

} // namespace Linux::
} // namespace StraitX::

#endif // STRAITX_LINUX_SCREEN_IMPL_HPP