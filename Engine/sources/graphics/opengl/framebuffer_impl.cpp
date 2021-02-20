#include <new>
#include "platform/memory.hpp"
#include "platform/opengl.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"

namespace StraitX{
namespace GL{

FramebufferImpl::FramebufferImpl(u32 handle, const RenderPass *const pass, const FramebufferProperties &props):
    m_Handle(handle),
    m_Size(props.Size)
{}

FramebufferImpl::FramebufferImpl(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props):
    m_Size(props.Size)
{
    // we don't care
    (void)owner;

    glGenFramebuffers(1, &m_Handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

    for(size_t i = 0; i<props.Attachments.Size(); ++i){
        u32 attachment = GPUTexture::IsDepthFormat(props.Attachments[i]->GetFormat()) ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0 + i;
        // XXX if it does not work, BindZero()
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, props.Attachments[i]->Handle().U32, 0);
    }
}

FramebufferImpl::~FramebufferImpl(){
    glDeleteFramebuffers(1, &m_Handle);
}

Vector2u FramebufferImpl::Size()const{
    return m_Size;
}

void FramebufferImpl::Bind()const{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
}

Framebuffer *FramebufferImpl::NewImpl(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props){
    return new(Memory::Alloc(sizeof(GL::FramebufferImpl))) GL::FramebufferImpl(owner, pass, props);
}

void FramebufferImpl::DeleteImpl(Framebuffer *framebuffer){
    framebuffer->~Framebuffer();
    Memory::Free(framebuffer);
}

}//namespace GL::
}//namespace StraitX::

