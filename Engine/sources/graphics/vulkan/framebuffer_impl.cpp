#include <new>
#include "platform/memory.hpp"
#include "platform/vulkan.hpp"
#include "core/algorithm.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/framebuffer_impl.hpp"

namespace StraitX{
namespace Vk{

FramebufferImpl::FramebufferImpl(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props):
    Owner(static_cast<Vk::LogicalGPUImpl*>(&owner)),
    FramebufferSize(props.Size)
{
    auto *attachments = (VkImageView *)alloca(props.Attachments.Size() * sizeof(VkImageView));

    for(size_t i = 0; i < props.Attachments.Size(); ++i){
        //don't lose the opportunity to store attachment pointers in a single loop
        Attachments.Push(props.Attachments[i]);

        attachments[i] = reinterpret_cast<VkImageView>(props.Attachments[i]->ViewHandle().U64);
    }

    VkFramebufferCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.renderPass = static_cast<const Vk::RenderPassImpl* const>(pass)->Handle;
    info.attachmentCount = props.Attachments.Size();
    info.pAttachments = attachments; 
    info.width = Size().x;
    info.height = Size().y;
    info.layers = 1;

    CoreFunctionAssert(vkCreateFramebuffer(Owner->Handle, &info, nullptr, &Handle), VK_SUCCESS, "Vk: FramebufferImpl: Can't create Framebuffer");
}

FramebufferImpl::~FramebufferImpl(){
    vkDestroyFramebuffer(Owner->Handle, Handle, nullptr);
}

Vector2u FramebufferImpl::Size()const{
    return FramebufferSize;
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

