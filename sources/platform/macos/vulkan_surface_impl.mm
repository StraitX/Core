#include "graphics/api/vulkan/vulkan.hpp"
#include "core/os/vulkan_surface.hpp"
#include "vulkan/vulkan_metal.h"
#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>
#include <vulkan/vulkan_macos.h>

Result VulkanSurface::Create(VkInstance owner, const OSWindowImpl &window){
    Owner = owner;

    NSView* view = (NSView*)window.View();
    assert([view isKindOfClass:[NSView class]]);

    if (![view.layer isKindOfClass:[CAMetalLayer class]])
    {
        [view setLayer:[CAMetalLayer layer]];
        [view setWantsLayer:YES];
    }

    VkMacOSSurfaceCreateInfoMVK info;
    info.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
    info.pNext = nullptr;
    info.flags = 0;
    info.pView = window.View();

    return ResultError(vkCreateMacOSSurfaceMVK(Owner, &info, nullptr, &Handle) != VK_SUCCESS);
}

void VulkanSurface::Destroy(){
    vkDestroySurfaceKHR(Owner, Handle, nullptr);
}
