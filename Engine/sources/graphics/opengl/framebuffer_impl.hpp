#ifndef STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP
#define STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP

#include "graphics/api/framebuffer.hpp"

namespace GL{

class FramebufferImpl: public Framebuffer{
private:
    u32 m_Handle;
public:

    FramebufferImpl(u32 handle, const RenderPass *const pass, const FramebufferProperties &props);

    FramebufferImpl(const RenderPass *const pass, const FramebufferProperties &props);

    ~FramebufferImpl();
    
    void Bind()const;

	u32 Handle()const;

    static Framebuffer *NewImpl(const RenderPass *const pass, const FramebufferProperties &props);

    static void DeleteImpl(Framebuffer *framebuffer);
};

}//namespace GL::

#endif//STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP