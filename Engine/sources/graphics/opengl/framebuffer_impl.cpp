#include <new>
#include "platform/memory.hpp"
#include "platform/opengl.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"

namespace StraitX{
namespace GL{

FramebufferImpl::FramebufferImpl(u32 handle, const RenderPass *const pass, const FramebufferProperties &props):
    Framebuffer(pass, props.Size),
    m_Handle(handle)
{}

FramebufferImpl::FramebufferImpl(const RenderPass *const pass, const FramebufferProperties &props):
    Framebuffer(pass, props.Size)
{
    glGenFramebuffers(1, &m_Handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

    for(size_t i = 0; i<props.Attachments.Size(); ++i){
        u32 attachment = IsDepthFormat(props.Attachments[i]->GetFormat()) ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0 + i;
        // XXX if it does not work, BindZero()
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, props.Attachments[i]->Handle().U32, 0);
    }
}

FramebufferImpl::~FramebufferImpl(){
    glDeleteFramebuffers(1, &m_Handle);
}

void FramebufferImpl::Bind()const{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
}

Framebuffer *FramebufferImpl::NewImpl(const RenderPass *const pass, const FramebufferProperties &props){
    return new(Memory::Alloc(sizeof(GL::FramebufferImpl))) GL::FramebufferImpl(pass, props);
}

void FramebufferImpl::DeleteImpl(Framebuffer *framebuffer){
    framebuffer->~Framebuffer();
    Memory::Free(framebuffer);
}

}//namespace GL::
}//namespace StraitX::

