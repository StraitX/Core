#ifndef STRAITX_VULKAN_SURFACE_HPP
#define STRAITX_VULKAN_SURFACE_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "platform/window.hpp"

namespace StraitX{

struct VulkanSurface{
    VkSurfaceKHR Handle = VK_NULL_HANDLE;
    VkInstance Owner = VK_NULL_HANDLE;

    Result Create(VkInstance owner, const PlatformWindow &window);

    void Destroy();
};

};//namespace StraitX::

#endif //STRAITX_VULKAN_SURFACE_HPP