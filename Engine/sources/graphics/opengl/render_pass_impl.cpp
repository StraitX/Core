#include <new>
#include "platform/memory.hpp"
#include "graphics/opengl/render_pass_impl.hpp"


namespace StraitX{
namespace GL{

RenderPassImpl::RenderPassImpl(const LogicalGPU &owner, const RenderPassProperties &props){
    (void)owner;
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

