#include "graphics/api/vulkan/framebuffer_impl.hpp"
#include "graphics/api/vulkan/texture_impl.hpp"
#include "graphics/api/vulkan/render_pass_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "core/os/memory.hpp"

namespace Vk{

FramebufferImpl::FramebufferImpl(const FramebufferProperties &props):
    m_Size(props.Size)
{
    VkImageView *attachments = SX_STACK_ARRAY_ALLOC(VkImageView, props.Attachments.Size());

    for(size_t i = 0; i < props.Attachments.Size(); ++i){
        //don't lose the opportunity to store attachment pointers in a single loop
        m_Attachments.Push(props.Attachments[i]);

        auto image = (Vk::Texture2DImpl*)props.Attachments[i];

        attachments[i] = image->ViewHandle();
    }

    VkFramebufferCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.renderPass = *(const Vk::RenderPassImpl*)props.Pass;
    info.attachmentCount = props.Attachments.Size();
    info.pAttachments = attachments; 
    info.width = props.Size.x;
    info.height = props.Size.y;
    info.layers = 1;

    SX_VK_ASSERT(vkCreateFramebuffer(GPUImpl::s_Instance, &info, nullptr, &m_Handle), "Vk: FramebufferImpl: Can't create Framebuffer");  
}

FramebufferImpl::~FramebufferImpl(){
    vkDestroyFramebuffer(GPUImpl::s_Instance, m_Handle, nullptr);
}

Vector2u FramebufferImpl::Size()const{
    return m_Size;
}

ConstSpan<Texture2D *> FramebufferImpl::Attachments()const{
    return m_Attachments;
}

}//namespace Vk::
