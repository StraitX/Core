#include <alloca.h>
#include "graphics/vulkan/swapchain.hpp"

namespace StraitX{
namespace Vk{


bool CheckFormat(Vk::LogicalDevice *presenter, const Vk::Surface &surface, VkSurfaceFormatKHR desired){
    u32 formats_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(presenter->Parent->Handle,surface.Handle, &formats_count, nullptr);
    VkSurfaceFormatKHR *formats = (VkSurfaceFormatKHR *)alloca(formats_count * sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(presenter->Parent->Handle,surface.Handle, &formats_count, formats);

    for(int i = 0; i<formats_count; i++){
        if(formats[i].colorSpace==desired.colorSpace && formats[i].format == desired.format)
            return true;
    }    
    return false;
}
    
VkPresentModeKHR BestMode(Vk::LogicalDevice *presenter, const Vk::Surface &surface){
    u32 modes_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(presenter->Parent->Handle, surface.Handle, &modes_count, nullptr);
    VkPresentModeKHR *modes = (VkPresentModeKHR *)alloca(modes_count * sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(presenter->Parent->Handle, surface.Handle, &modes_count, modes);

    bool modesAvailable[VK_PRESENT_MODE_END_RANGE_KHR + 1]; // index is the same as VkPresentModeKHR enum element number
    VkPresentModeKHR best_mode;
    for(int i = 0; i<modes_count; i++){
        modesAvailable[modes[i]]=true;
    }
    if(modesAvailable[VK_PRESENT_MODE_MAILBOX_KHR])
        best_mode = VK_PRESENT_MODE_MAILBOX_KHR;
    else if(modesAvailable[VK_PRESENT_MODE_FIFO_RELAXED_KHR])
        best_mode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
    else if(modesAvailable[VK_PRESENT_MODE_FIFO_KHR])
        best_mode = VK_PRESENT_MODE_FIFO_KHR;
    else // is guaranteed to be supported
        best_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;

    return best_mode;
}


Error Swapchain::Create(VkSurfaceFormatKHR format, Vk::LogicalDevice*owner, const Vk::Surface &surface){
    Owner = owner;

    VkBool32 support = false;             //  We assume that RenderAPI won't allow to create a device without GraphicsQueue
    vkGetPhysicalDeviceSurfaceSupportKHR(Owner->Parent->Handle, Owner->Parent->GraphicsQueueFamily, surface.Handle, &support);

    if(support == false)
        return Error::Unsupported;

    if(!CheckFormat(owner, surface, format))
        return Error::Unsupported;

    Colorspace = format.colorSpace;
    Format = format.format;

    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Owner->Parent->Handle, surface.Handle, &capabilities);

    u32 desired_images = 2;
    if(capabilities.maxImageCount >=3)
        desired_images = 3;

    if(capabilities.minImageCount > MaxSwapchainImages)
        return Error::Unsupported;


    VkSwapchainCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.clipped = true;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.surface = surface.Handle;
    info.oldSwapchain = VK_NULL_HANDLE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.preTransform = VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR;
    info.presentMode = BestMode(Owner, surface);
    info.minImageCount = desired_images;
    info.imageArrayLayers = 1;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.imageExtent = capabilities.currentExtent;
    info.imageFormat = Format;
    info.imageColorSpace = Colorspace;

    if(vkCreateSwapchainKHR(Owner->Handle,&info,nullptr,&Handle) != VK_SUCCESS)
        return Error::Failure;

    return Error::Success;
}


void Swapchain::Destroy(){
    vkDestroySwapchainKHR(Owner->Handle, Handle, nullptr);
}

};//namespace Vk::
};//namespace StraitX::

