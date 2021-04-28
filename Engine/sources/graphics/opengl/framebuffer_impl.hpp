#ifndef STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP
#define STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP

#include "graphics/api/framebuffer.hpp"

namespace StraitX{
namespace GL{

class FramebufferImpl: public Framebuffer{
private:
    u32 m_Handle;
    Vector2u m_Size;
public:

    FramebufferImpl(u32 handle, const RenderPass *const pass, const FramebufferProperties &props);

    FramebufferImpl(const RenderPass *const pass, const FramebufferProperties &props);

    ~FramebufferImpl();

    virtual Vector2u Size()const override;

    void Bind()const;

    static Framebuffer *NewImpl(const RenderPass *const pass, const FramebufferProperties &props);

    static void DeleteImpl(Framebuffer *framebuffer);
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_FRAMEBUFFER_IMPL_HPP