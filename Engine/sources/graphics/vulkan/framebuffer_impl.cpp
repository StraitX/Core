#include <new>
#include "platform/memory.hpp"
#include "platform/vulkan.hpp"
#include "core/algorithm.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/framebuffer_impl.hpp"

namespace StraitX{
namespace Vk{

FramebufferImpl::FramebufferImpl(FramebufferImpl &&other)
{
    Swap(*this, other);
}

FramebufferImpl::FramebufferImpl(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props):
    Owner(static_cast<Vk::LogicalGPUImpl*>(&owner))
{
    auto *attachments = (VkImageView *)alloca(props.Attachments.Size() * sizeof(VkImageView));

    for(size_t i = 0; i < props.Attachments.Size(); ++i){
        attachments[i] = reinterpret_cast<VkImageView>(props.Attachments[i]->ViewHandle().U64);
    }

    VkFramebufferCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.renderPass = static_cast<const Vk::RenderPassImpl* const>(pass)->Handle;
    info.attachmentCount = props.Attachments.Size();
    info.pAttachments = attachments; 
    info.width = props.Size.x;
    info.height = props.Size.y;
    info.layers = 1;

    CoreFunctionAssert(vkCreateFramebuffer(Owner->Handle, &info, nullptr, &Handle), VK_SUCCESS, "Vk: FramebufferImpl: Can't create Framebuffer");
}

FramebufferImpl::~FramebufferImpl(){
    if(Handle)
        vkDestroyFramebuffer(Owner->Handle, Handle, nullptr);
}
FramebufferImpl &FramebufferImpl::operator=(FramebufferImpl &&other){
    const_cast<Vk::LogicalGPUImpl *&>(Owner) = other.Owner;
    Handle = other.Handle;

    const_cast<Vk::LogicalGPUImpl *&>(other.Owner) = nullptr;
    other.Handle = VK_NULL_HANDLE;
    return *this;
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

