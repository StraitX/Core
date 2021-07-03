#define VK_USE_PLATFORM_WIN32_KHR
#include "platform/vulkan.hpp"
#include "platform/vulkan_surface.hpp"

namespace StraitX {

Result VulkanSurface::Create(VkInstance owner, const PlatformWindow& window) {
    Owner = owner;
    VkWin32SurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.hinstance = (HINSTANCE)GetModuleHandle(nullptr);
    info.hwnd = window.Impl().Handle();

    return ResultError(vkCreateWin32SurfaceKHR(Owner, &info, nullptr, &Handle) != VK_SUCCESS);
}

void VulkanSurface::Destroy() {
    vkDestroySurfaceKHR(Owner, Handle, nullptr);
}

}//namespace StraitX::