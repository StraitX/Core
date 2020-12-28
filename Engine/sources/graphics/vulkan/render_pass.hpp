#ifndef STRAITX_VULKAN_RENDER_PASS_HPP
#define STRAITX_VULKAN_RENDER_PASS_HPP


#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "core/array_ptr.hpp"
#include "graphics/vulkan/logical_device.hpp"

namespace StraitX{
namespace Vk{



struct RenderPass{
    VkRenderPass Handle = VK_NULL_HANDLE;
    Vk::LogicalDevice *Owner = nullptr;

    Result Create(Vk::LogicalDevice *owner, const ArrayPtr<VkAttachmentDescription> &descriptions, const ArrayPtr<VkSubpassDescription> &subpass);

    void Destroy();
};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_RENDER_PASS_HPP