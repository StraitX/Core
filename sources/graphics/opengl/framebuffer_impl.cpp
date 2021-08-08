#include <new>
#include "core/os/memory.hpp"
#include "core/os/opengl.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"

namespace GL{

FramebufferImpl::FramebufferImpl(u32 handle, const RenderPass *const pass, const FramebufferProperties &props):
    Framebuffer(pass),
    Handle(handle),
	AttachmentsSize(props.Size)
{}

FramebufferImpl::FramebufferImpl(const RenderPass *const pass, const FramebufferProperties &props):
    Framebuffer(pass),
	AttachmentsSize(props.Size)
{
    glGenFramebuffers(1, &Handle);
    glBindFramebuffer(GL_FRAMEBUFFER, Handle);

    for(size_t i = 0; i<props.Attachments.Size(); ++i){
        u32 attachment = IsDepthFormat(props.Attachments[i]->Format()) ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0 + i;
        // XXX if it does not work, BindZero()
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, props.Attachments[i]->Handle().U32, 0);
    }
}

FramebufferImpl::~FramebufferImpl(){
    glDeleteFramebuffers(1, &Handle);
}

void FramebufferImpl::Bind()const{
    glBindFramebuffer(GL_FRAMEBUFFER, Handle);
}


Vector2u FramebufferImpl::Size()const{
	return AttachmentsSize;
}

Framebuffer *FramebufferImpl::NewImpl(const RenderPass *const pass, const FramebufferProperties &props){
    return new GL::FramebufferImpl(pass, props);
}

void FramebufferImpl::DeleteImpl(Framebuffer *framebuffer){
    delete framebuffer;
}

}//namespace GL::