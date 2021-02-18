#ifndef STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP
#define STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP

#include "graphics/api/framebuffer.hpp"

namespace StraitX{
namespace GL{

struct FramebufferImpl: Framebuffer{
    u32 Handle;

    FramebufferImpl(u32 handle);

    FramebufferImpl(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props);

    ~FramebufferImpl();

    void Bind()const;

    static Framebuffer *NewImpl(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props);

    static void DeleteImpl(Framebuffer *framebuffer);
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP