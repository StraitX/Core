#ifndef STRAITX_VULKAN_SWAPCHAIN_HPP
#define STRAITX_VULKAN_SWAPCHAIN_HPP

#include "platform/vulkan.hpp"
#include "platform/error.hpp"
#include "platform/vk_surface.hpp"
#include "core/math/vector2.hpp"
#include "graphics/vulkan/logical_device.hpp"

namespace StraitX{
namespace Vk{

constexpr size_t MaxSwapchainImages = 3;

struct Swapchain{
    Vk::Surface *Surface = nullptr;
    Vk::LogicalDevice *Owner = nullptr;
    VkSwapchainKHR Handle = VK_NULL_HANDLE;

    VkColorSpaceKHR Colorspace;
    VkFormat Format;

    Vector2u SurfaceSize = {0,0}; 
    u32 ImagesCount = 0;
    VkImage Images[MaxSwapchainImages];
    VkImageView ImageViews[MaxSwapchainImages];
    VkFramebuffer Framebuffers[MaxSwapchainImages];

    Error Create(Vk::Surface *surface, Vk::LogicalDevice *owner, VkSurfaceFormatKHR format);

    Error CreateChain();

    Error CreateImageViews();

    void DestroyImageViews();

    void DestroyChain();

    void Destroy();
};

};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_SWAPCHAIN_HPP