#ifndef STRAITX_MACOS_OPENGL_CONTEXT_IMPL_HPP
#define STRAITX_MACOS_OPENGL_CONTEXT_IMPL_HPP

#include "platform/window.hpp"
#include "platform/macos/window_impl.hpp"

namespace StraitX{
namespace MacOS{

struct OpenGLContextImpl{
    void *Handle = nullptr;

    Result Create(WindowImpl &window, const Version &version);

    Result CreateDummy();

    void Destroy();

    void DestroyDummy();

    Result MakeCurrent();

    void SwapBuffers();
};

}//namespace MacOS::
}//namespace StraitX::

#endif//STRAITX_MACOS_OPENGL_CONTEXT_IMPL_HPP