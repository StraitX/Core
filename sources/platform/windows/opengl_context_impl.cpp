#include <Windows.h>
#undef min
#include <limits>
#include "core/os/windows/opengl_context_impl.hpp"
#include "core/os/windows/wgl.hpp"

namespace Windows{

constexpr size_t MaxFormatsCount = 64;

Result OpenGLContextImpl::Create(const WindowImpl& window, const Version& version) {
	m_WindowHandle = window.Handle();
	m_DeviceContext = GetDC(m_WindowHandle);

	PIXELFORMATDESCRIPTOR pfd = {};

	const int pixel_format_attrib_list[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		0
    };
    int context_attrib_list[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, version.Major,
        WGL_CONTEXT_MINOR_VERSION_ARB, version.Minor,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        0
    };

    int pixel_formats[MaxFormatsCount];
    UINT got_formats = 0;

    wglChoosePixelFormatARB(m_DeviceContext, pixel_format_attrib_list, NULL, MaxFormatsCount, pixel_formats, (UINT*)&got_formats);

	if (!got_formats)return Result::Unsupported;

	size_t best_index = ChooseBestFormat(pixel_formats, got_formats);

    SetPixelFormat(m_DeviceContext, pixel_formats[best_index], &pfd);

    m_Handle = wglCreateContextAttribsARB(m_DeviceContext, 0, context_attrib_list);
	return ResultError(m_Handle == nullptr);
}

Result OpenGLContextImpl::CreateLegacy(const WindowImpl &window) {
	m_WindowHandle = window.Handle();
	m_DeviceContext = GetDC(m_WindowHandle);

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pfi = ChoosePixelFormat(m_DeviceContext, &pfd);

	if (!pfi)
		return Result::Unsupported;

	SetPixelFormat(m_DeviceContext, pfi, &pfd);
	
	m_Handle = wglCreateContext(m_DeviceContext);
	return ResultError(m_Handle == nullptr);
}

void OpenGLContextImpl::Destroy() {
	ReleaseDC(m_WindowHandle, m_DeviceContext);
	wglDeleteContext(m_Handle);
}

void OpenGLContextImpl::DestroyLegacy() {
	Destroy();
}

Result OpenGLContextImpl::MakeCurrent() {
	wglMakeCurrent(m_DeviceContext, m_Handle);
	return ResultError(wglGetCurrentContext() == nullptr);
}

void OpenGLContextImpl::SwapBuffers() {
	::SwapBuffers(m_DeviceContext);
}

void OpenGLContextImpl::Resize(u32 width, u32 height){
	(void)width;
	(void)height;
}

size_t OpenGLContextImpl::ChooseBestFormat(int formats[], unsigned int count) {
	size_t best = 0;
	float best_score = std::numeric_limits<float>::min();

	for (size_t i = 0; i < count; i++) {
        const int attributes[] =
        {
			WGL_RED_BITS_ARB,
			WGL_GREEN_BITS_ARB,
			WGL_BLUE_BITS_ARB,
			WGL_ALPHA_BITS_ARB,
			WGL_DEPTH_BITS_ARB,
			WGL_STENCIL_BITS_ARB,

            WGL_SAMPLES_ARB
        };
		int values[sizeof(attributes) / sizeof(const int)];

		wglGetPixelFormatAttribivARB(m_DeviceContext, formats[i], PFD_MAIN_PLANE, count, attributes, values);

		float score = attributes[0] / 8.f + attributes[1] / 8.f + attributes[2] / 8.f + attributes[3] / 8.f + attributes[4] / 24.f + attributes[5] / 8.f + attributes[6] / 1.f - 7.f;
		if (score > best_score) {
			best_score = score;
			best = i;
		}
	}
	return best;
}

}// namespace Windows::