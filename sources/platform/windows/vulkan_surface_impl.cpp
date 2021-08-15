#define VK_USE_PLATFORM_WIN32_KHR
#include "core/os/vulkan.hpp"
#include "core/os/vulkan_surface.hpp"

Result VulkanSurface::Create(VkInstance owner, const OSWindowImpl& window) {
    Owner = owner;
    VkWin32SurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.hinstance = (HINSTANCE)GetModuleHandle(nullptr);
    info.hwnd = window.Handle();

    return ResultError(vkCreateWin32SurfaceKHR(Owner, &info, nullptr, &Handle) != VK_SUCCESS);
}

void VulkanSurface::Destroy() {
    vkDestroySurfaceKHR(Owner, Handle, nullptr);
}