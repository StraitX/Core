#ifndef STRAITX_VULKAN_SURFACE_HPP
#define STRAITX_VULKAN_SURFACE_HPP

#include "core/os/vulkan.hpp"
#include "core/result.hpp"
#include "core/os/window.hpp"

struct VulkanSurface{
    VkSurfaceKHR Handle = VK_NULL_HANDLE;
    VkInstance Owner = VK_NULL_HANDLE;

    Result Create(VkInstance owner, const PlatformWindowImpl &window);

    void Destroy();
};

#endif //STRAITX_VULKAN_SURFACE_HPP