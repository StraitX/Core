#include <new>
#include "platform/memory.hpp"
#include "platform/vulkan.hpp"
#include "core/algorithm.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/framebuffer_impl.hpp"

namespace StraitX{
namespace Vk{

FramebufferImpl::FramebufferImpl(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props):
    Framebuffer(pass, props),
    Owner(static_cast<Vk::LogicalGPUImpl*>(&owner))
{
    auto *attachments = (VkImageView *)alloca(Attachments().Size() * sizeof(VkImageView));

    for(size_t i = 0; i < Attachments().Size(); ++i){
        attachments[i] = reinterpret_cast<VkImageView>(Attachments()[i]->ViewHandle().U64);
    }

    VkFramebufferCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.renderPass = static_cast<const Vk::RenderPassImpl* const>(pass)->Handle;
    info.attachmentCount = Attachments().Size();
    info.pAttachments = attachments; 
    info.width = Size().x;
    info.height = Size().y;
    info.layers = 1;

    CoreFunctionAssert(vkCreateFramebuffer(Owner->Handle, &info, nullptr, &Handle), VK_SUCCESS, "Vk: FramebufferImpl: Can't create Framebuffer");
}

FramebufferImpl::~FramebufferImpl(){
    vkDestroyFramebuffer(Owner->Handle, Handle, nullptr);
}

Framebuffer *FramebufferImpl::NewImpl(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props){
    return new(Memory::Alloc(sizeof(Vk::FramebufferImpl))) Vk::FramebufferImpl(owner, pass, props);
}

void FramebufferImpl::DeleteImpl(Framebuffer *framebuffer){
    framebuffer->~Framebuffer();
    Memory::Free(framebuffer);
}

}//namespace Vk::
}//namespace StraitX::

