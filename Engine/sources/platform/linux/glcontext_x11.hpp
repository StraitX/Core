#ifndef STRAITX_GLCONTEXT_X11_HPP
#define STRAITX_GLCONTEXT_X11_HPP

#include "platform/linux/display_x11.hpp"
#include "platform/linux/window_x11.hpp"
#include "platform/types.hpp"

struct __GLXcontextRec;

namespace StraitX{
namespace Linux{

class GLContextX11{
private:
    WindowX11 &m_Window;
    __GLXcontextRec *m_Handle = nullptr;
public:
    GLContextX11(WindowX11 &window);

    Result Create(const Version &version);

    void Destory();

    Result MakeCurrent();

    void SwapBuffers();

};

};
}; // namespace StraitX::

#endif // STRAITX_GLCONTEXT_X11_HPP