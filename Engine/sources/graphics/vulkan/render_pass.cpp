#include "graphics/vulkan/render_pass.hpp"


namespace StraitX{
namespace Vk{

Result RenderPass::Create(Vk::LogicalDevice *owner, const ArrayPtr<VkAttachmentDescription> &attachments, const ArrayPtr<VkSubpassDescription> &subpass){
    Owner = owner;

    VkRenderPassCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.subpassCount = subpass.Size;
    info.pSubpasses = subpass.Pointer;
    info.attachmentCount = attachments.Size;
    info.pAttachments = attachments.Pointer;
    info.dependencyCount = 0;
    info.pDependencies = nullptr;
    
    return ResultError(vkCreateRenderPass(Owner->Handle, &info, nullptr, &Handle) != VK_SUCCESS);
}

void RenderPass::Destroy(){
    vkDestroyRenderPass(Owner->Handle, Handle, nullptr);
}

};//namespace Vk::
};//namespace StraitX::
