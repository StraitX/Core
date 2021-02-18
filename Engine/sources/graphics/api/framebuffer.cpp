#include "platform/compiler.hpp"
#include "graphics/api/framebuffer.hpp"

namespace StraitX{

Framebuffer::VTable Framebuffer::s_VTable;

Framebuffer::Framebuffer(const RenderPass *const pass, const FramebufferProperties &props):
    m_Size(props.Size)
{
#ifdef SX_DEBUG
    CoreAssert(pass->Attachments().Size() == props.Attachments.Size(),"Framebuffer: attachments are not compatible with RenderPass");

    for(size_t i = 0; i<pass->Attachments().Size(); ++i){
        CoreAssert(pass->Attachments()[i].Format == props.Attachments[i]->GetFormat(), "attachment format is not compatible with RenderPass");
    }
#else
    (void)pass;
    (void)props;
#endif
}

}//namespace StraitX::