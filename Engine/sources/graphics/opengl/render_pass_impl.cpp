#include <new>
#include "platform/memory.hpp"
#include "graphics/opengl/render_pass_impl.hpp"


namespace StraitX{
namespace GL{

RenderPassImpl::RenderPassImpl(const LogicalGPU &owner, const RenderPassProperties &props){
    (void)owner;

    bool has_depth = false;

    for(const auto &a: props.Attachments){
        if(a.Format == GPUTexture::Format::Depth24Stencil8){
            CoreAssert(!has_depth, "GL: RenderPassImpl: more than one depth attachment is not supported");
            DepthAttachment = a;
            has_depth = true;
        }else{
            ColorAttachments.Push(a);
        }
    }
}

RenderPassImpl::~RenderPassImpl(){
    (void)0;
}

RenderPass *RenderPassImpl::NewImpl(const LogicalGPU &owner, const RenderPassProperties &props){
    return new(Memory::Alloc(sizeof(GL::RenderPassImpl))) GL::RenderPassImpl(owner, props);
}

void RenderPassImpl::DeleteImpl(RenderPass *pass){
    pass->~RenderPass();
    Memory::Free(pass);
}

}//namespace GL::
}//namespace StraitX::

