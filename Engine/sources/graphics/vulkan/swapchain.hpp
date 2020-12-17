#ifndef STRAITX_VULKAN_SWAPCHAIN_HPP
#define STRAITX_VULKAN_SWAPCHAIN_HPP

#include "platform/vulkan.hpp"
#include "platform/error.hpp"
#include "platform/vk_surface.hpp"
#include "graphics/vulkan/logical_device.hpp"

namespace StraitX{
namespace Vk{

constexpr size_t MaxSwapchainImages = 3;

struct Swapchain{
    Vk::LogicalDevice *Owner = nullptr;
    VkSwapchainKHR Handle = VK_NULL_HANDLE;
    VkColorSpaceKHR Colorspace;
    VkFormat Format;
    size_t ImagesCount = 0;
    VkImage Images[MaxSwapchainImages];
    VkImageView ImageVies[MaxSwapchainImages];
    VkFramebuffer Framebuffers[MaxSwapchainImages];

    Error Create(VkSurfaceFormatKHR format, Vk::LogicalDevice *owner, const Vk::Surface &surface);

    void Destroy();
};

};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_SWAPCHAIN_HPP