#include "graphics/vulkan/framebuffer.hpp"

namespace StraitX{
namespace Vk{

Result Framebuffer::Create(Vk::RenderPass *render_pass, const Vector2u &size, const ArrayPtr<VkImageView> &attachments){
    RenderPass = render_pass;
    Size = size;

    VkFramebufferCreateInfo info; 
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.width = Size.x;
    info.height = Size.y;
    info.layers = 1;
    info.renderPass = render_pass->Handle;
    info.attachmentCount = attachments.Size;
    info.pAttachments = attachments.Pointer;

    return ResultError(vkCreateFramebuffer(render_pass->Owner->Handle, &info, nullptr, &Handle) != VK_SUCCESS);
}

void Framebuffer::Destroy(){
    vkDestroyFramebuffer(RenderPass->Owner->Handle, Handle, nullptr);
}


};//namespace Vk::
};//namespace StraitX::