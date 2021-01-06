#define VK_USE_PLATFORM_XLIB_KHR
#include "platform/vulkan.hpp"
// undef to prevent X11 macros collapse
#undef Success
#undef None
#undef KeyRelease
#undef KeyPress
#include "platform/vk_surface.hpp"


namespace StraitX{
namespace Vk{


Result Surface::Create(VkInstance owner, const Window &window){
    Owner = owner;
    VkXlibSurfaceCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.dpy = window.Impl().Display.Handle();
    info.window = window.Impl().Handle;

    return ResultError(vkCreateXlibSurfaceKHR(Owner, &info, nullptr, &Handle) != VK_SUCCESS);
}

void Surface::Destroy(){
    vkDestroySurfaceKHR(Owner, Handle, nullptr);
}

};//namespace Vk::
};//namespace StraitX::



