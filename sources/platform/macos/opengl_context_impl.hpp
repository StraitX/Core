#ifndef STRAITX_MACOS_OPENGL_CONTEXT_IMPL_HPP
#define STRAITX_MACOS_OPENGL_CONTEXT_IMPL_HPP

#include "core/types.hpp"
#include "core/result.hpp"
#include "platform/macos/window_impl.hpp"

namespace MacOS{

class OpenGLContextImpl{
private:
    void *Handle = nullptr;
    void *Format = nullptr;
public:
    OpenGLContextImpl() = default;

    Result Create(const WindowImpl &window, const Version &version);

    Result CreateLegacy(const WindowImpl &window);

    void Destroy();

    void DestroyLegacy();

    Result MakeCurrent();

    void SwapBuffers();

	void Resize(u32 width, u32 height);

};

}//namespace MacOS::

#endif //STRAITX_MACOS_OPENGL_CONTEXT_IMPL_HPP