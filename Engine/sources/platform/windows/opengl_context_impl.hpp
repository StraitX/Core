#ifndef STRAITX_WINDOWS_OPENGL_CONTEXT_IMPL_HPP
#define STRAITX_WINDOWS_OPENGL_CONTEXT_IMPL_HPP

#include "platform/windows/window_impl.hpp"
#include "platform/result.hpp"
#include "platform/types.hpp"

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

    Result CreateDummy();

    void Destroy();

    void DestroyDummy();

    Result MakeCurrent();

    void SwapBuffers();

	void Resize(u32 width, u32 height);
private:
    size_t ChooseBestFormat(int formats[], unsigned int count);
};  

}//namespace Windows::

#endif // STRAITX_WINDOWS_OPENGL_CONTEXT_IMPL_HPP