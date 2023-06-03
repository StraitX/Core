#include "graphics/api/vulkan/vulkan.hpp"
#include "core/os/vulkan_surface.hpp"
#include "platform/linux/display_server.hpp"

namespace X11{
#include <vulkan/vulkan_xlib.h>
}//namespace X11::

Result VulkanSurface::Create(VkInstance owner, const OSWindowImpl &window){
    Owner = owner;
    X11::VkXlibSurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.dpy = Linux::DisplayServer::Handle;
    info.window = window.Handle();

    return ResultError(vkCreateXlibSurfaceKHR(Owner, &info, nullptr, &Handle) != VK_SUCCESS);
}

void VulkanSurface::Destroy(){
    vkDestroySurfaceKHR(Owner, Handle, nullptr);
}
