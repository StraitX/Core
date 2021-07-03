#include <new>
#include "platform/memory.hpp"
#include "graphics/opengl/render_pass_impl.hpp"

namespace GL{

RenderPassImpl::RenderPassImpl(const RenderPassProperties &props){
    (void)props;
}

RenderPassImpl::~RenderPassImpl(){
    (void)0;
}

RenderPass *RenderPassImpl::NewImpl(const RenderPassProperties &props){
    return new(Memory::Alloc(sizeof(GL::RenderPassImpl))) GL::RenderPassImpl(props);
}

void RenderPassImpl::DeleteImpl(RenderPass *pass){
    pass->~RenderPass();
    Memory::Free(pass);
}

}//namespace GL::