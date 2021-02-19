#include "platform/compiler.hpp"
#include "graphics/api/framebuffer.hpp"

namespace StraitX{

Framebuffer::VTable Framebuffer::s_VTable;

Framebuffer::Framebuffer(const RenderPass *const pass, const FramebufferProperties &props):
    m_Size(props.Size)
{
    CoreAssert(pass->Attachments().Size() == props.Attachments.Size(), "Framebuffer: Incompatible RenderPass");

    for(size_t i = 0; i<props.Attachments.Size(); ++i){
        m_Attachments.Push(props.Attachments[i]);
    }
}

}//namespace StraitX::