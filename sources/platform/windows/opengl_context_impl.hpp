#ifndef STRAITX_WINDOWS_OPENGL_CONTEXT_IMPL_HPP
#define STRAITX_WINDOWS_OPENGL_CONTEXT_IMPL_HPP

#include "core/os/windows/window_impl.hpp"
#include "core/result.hpp"
#include "core/types.hpp"

struct HGLRC__;
struct HDC__;

namespace Windows{

class OpenGLContextImpl {
private:
    HGLRC__ *m_Handle = nullptr;
    HDC__ *m_DeviceContext = nullptr;
    HWND__ *m_WindowHandle = nullptr;
public:
    OpenGLContextImpl() = default;

    Result Create(const WindowImpl& window, const Version& version);

    Result CreateLegacy(const WindowImpl &window);

    void Destroy();

    void DestroyLegacy();

    Result MakeCurrent();

    void SwapBuffers();

	void Resize(u32 width, u32 height);
private:
    size_t ChooseBestFormat(int formats[], unsigned int count);
};  

}//namespace Windows::

#endif // STRAITX_WINDOWS_OPENGL_CONTEXT_IMPL_HPP