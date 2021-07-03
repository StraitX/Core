#define VK_USE_PLATFORM_XLIB_KHR
#include "platform/vulkan.hpp"
// undef to prevent X11 macros collapse
#undef Success
#undef None
#undef KeyRelease
#undef KeyPress
#include "platform/vulkan_surface.hpp"

namespace Linux{
extern ::Display *s_Display;
}//namespace Linux::

Result VulkanSurface::Create(VkInstance owner, const PlatformWindow &window){
    Owner = owner;
    VkXlibSurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.dpy = Linux::s_Display;
    info.window = window.Impl().Handle;

    return ResultError(vkCreateXlibSurfaceKHR(Owner, &info, nullptr, &Handle) != VK_SUCCESS);
}

void VulkanSurface::Destroy(){
    vkDestroySurfaceKHR(Owner, Handle, nullptr);
}
