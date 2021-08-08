#define VK_USE_PLATFORM_XLIB_KHR
#include "core/os/vulkan.hpp"
// undef to prevent X11 macros collapse
#undef Success
#undef None
#undef KeyRelease
#undef KeyPress
#undef FocusIn
#undef FocusOut
#include "core/os/vulkan_surface.hpp"
#include "platform/linux/display_server.hpp"

Result VulkanSurface::Create(VkInstance owner, const PlatformWindowImpl &window){
    Owner = owner;
    VkXlibSurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.dpy = Linux::DisplayServer::Handle;
    info.window = window.Handle;

    return ResultError(vkCreateXlibSurfaceKHR(Owner, &info, nullptr, &Handle) != VK_SUCCESS);
}

void VulkanSurface::Destroy(){
    vkDestroySurfaceKHR(Owner, Handle, nullptr);
}
