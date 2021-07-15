#ifndef STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP
#define STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP

#include "graphics/framebuffer.hpp"

namespace GL{

struct FramebufferImpl: public Framebuffer{
    u32 Handle;
	Vector2u AttachmentsSize;

    FramebufferImpl(u32 handle, const RenderPass *const pass, const FramebufferProperties &props);

    FramebufferImpl(const RenderPass *const pass, const FramebufferProperties &props);

    ~FramebufferImpl();
    
    void Bind()const;

	virtual Vector2u Size()const override;

    static Framebuffer *NewImpl(const RenderPass *const pass, const FramebufferProperties &props);

    static void DeleteImpl(Framebuffer *framebuffer);
};

}//namespace GL::

#endif//STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP