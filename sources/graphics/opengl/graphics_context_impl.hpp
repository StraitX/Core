#ifndef STRAITX_OPENGL_GRAPHICS_CONTEXT_IMPL_HPP
#define STRAITX_OPENGL_GRAPHICS_CONTEXT_IMPL_HPP

#include "core/os/opengl_context.hpp"
#include "core/os/opengl.hpp"
#include "core/raw_var.hpp"
#include "graphics/graphics_context.hpp"
#include "graphics/opengl/render_pass_impl.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"

namespace GL{

class GraphicsContextImpl: public GraphicsContext{
private:
	static GLenum s_IndexTypeTable[];
private:
	OpenGLContext m_OpenGLContext;
    Version m_LoadedOpenGLVersion = {};
    const char *m_VendorString    = nullptr;
    const char *m_RendererString  = nullptr;
    const char *m_VersionString   = nullptr;
    GPUVendor m_Vendor = GPUVendor::Unknown;

    s32 m_MaxTextureUnits = 0;
    s32 m_MaxUniformBufferBindings = 0;

	RawVar<GL::RenderPassImpl> m_FramebufferPass;
    RawVar<GL::FramebufferImpl> m_DefaultFramebuffer;
public:
	static GraphicsContextImpl s_Instance;

	virtual Result Initialize()override;

	virtual void Finalize()override;

	virtual void ExecuteCmdBuffer(const GPUCommandBuffer &cmd_buffer)override;

	virtual void SwapBuffers()override;

	virtual void ResizeSwapchain(u32 width, u32 height)override;

	virtual const Framebuffer *CurrentFramebuffer()override;

	virtual const RenderPass *FramebufferPass()override;

	Version LoadedVersion()const;

	u32 MaxTextureUnits()const;
};

}//namespace GL::

#endif//STRAITX_OPENGL_GRAPHICS_CONTEXT_IMPL_HPP