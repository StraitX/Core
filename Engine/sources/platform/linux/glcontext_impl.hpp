#ifndef STRAITX_LINUX_GLCONTEXT_IMPL_HPP
#define STRAITX_LINUX_GLCONTEXT_IMPL_HPP

#include "platform/linux/window_impl.hpp"
#include "platform/types.hpp"

struct __GLXcontextRec;

namespace StraitX{
namespace Linux{

class GLContextImpl{
private:
    __GLXcontextRec *m_Handle = nullptr;
    unsigned long m_WindowHandle = 0;
public:
    GLContextImpl() = default;

    Result Create(WindowImpl &window, const Version &version);

    Result CreateDummy(const Version &version);

    void Destroy();

    void DestroyDummy();

    Result MakeCurrent();

    void SwapBuffers();

};

} // namespace Linux::
} // namespace StraitX::

#endif // STRAITX_LINUX_GLCONTEXT_IMPL_HPP