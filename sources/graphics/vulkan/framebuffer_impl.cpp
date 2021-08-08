#include <new>
#include "core/os/memory.hpp"
#include "core/os/vulkan.hpp"
#include "core/algorithm.hpp"
#include "graphics/vulkan/gpu.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/framebuffer_impl.hpp"
#include "graphics/vulkan/debug.hpp"

namespace Vk{

FramebufferImpl::FramebufferImpl(const RenderPass *const pass, const FramebufferProperties &props):
    Framebuffer(pass),
	m_Size(props.Size)
{
    auto *attachments = (VkImageView *)alloca(props.Attachments.Size() * sizeof(VkImageView));

    for(size_t i = 0; i < props.Attachments.Size(); ++i){
        //don't lose the opportunity to store attachment pointers in a single loop
        m_Attachments.Push(props.Attachments[i]);

        attachments[i] = reinterpret_cast<VkImageView>(props.Attachments[i]->ViewHandle().U64);
    }

    VkFramebufferCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.renderPass = static_cast<const Vk::RenderPassImpl* const>(pass)->Handle();
    info.attachmentCount = props.Attachments.Size();
    info.pAttachments = attachments; 
    info.width = props.Size.x;
    info.height = props.Size.y;
    info.layers = 1;

    SX_VK_ASSERT(vkCreateFramebuffer(GPU::Get().Handle(), &info, nullptr, &m_Handle), "Vk: FramebufferImpl: Can't create Framebuffer");
}

FramebufferImpl::~FramebufferImpl(){
    vkDestroyFramebuffer(GPU::Get().Handle(), m_Handle, nullptr);
}

Framebuffer *FramebufferImpl::NewImpl(const RenderPass *const pass, const FramebufferProperties &props){
    return new Vk::FramebufferImpl(pass, props);
}

void FramebufferImpl::DeleteImpl(Framebuffer *framebuffer){
    delete framebuffer;
}

}//namespace Vk::