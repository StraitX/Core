#include "platform/memory.hpp"
#include "platform/memory.hpp"
#include "core/log.hpp"
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

    // TODO: Fix potential stack corruption
    bool modes_available[5] = {0}; // index is the same as VkPresentModeKHR enum element number

    for(int i = 0; i<modes_count; i++){
        modes_available[modes[i]]=true;
    }

    if(modes_available[VK_PRESENT_MODE_MAILBOX_KHR])
        return VK_PRESENT_MODE_MAILBOX_KHR;
    else if(modes_available[VK_PRESENT_MODE_FIFO_RELAXED_KHR])
        return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
    else if(modes_available[VK_PRESENT_MODE_FIFO_KHR])
        return VK_PRESENT_MODE_FIFO_KHR;
    else // is guaranteed to be supported
        return VK_PRESENT_MODE_IMMEDIATE_KHR;
}


Result Swapchain::Create(Vk::Surface *surface, Vk::LogicalDevice *owner, VkSurfaceFormatKHR format){
    Owner = owner;
    Surface = surface;

    VkBool32 support = false;             //  We assume that RenderAPI won't allow to create a device without GraphicsQueue
    vkGetPhysicalDeviceSurfaceSupportKHR(Owner->Parent->Handle, Owner->Parent->GraphicsQueueFamily, Surface->Handle, &support);

    if(support == false)
        return Result::Unsupported;

    if(!CheckFormat(Owner, *Surface, format))
        return Result::Unsupported;

    Colorspace = format.colorSpace;
    Format = format.format;
    
    Result chain = CreateChain();
    if(chain != Result::Success)
        return chain;

    Result views = CreateImageViews();
    if(views != Result::Success)
        return views;

    return Result::Success;
}
Result Swapchain::CreateChain(){
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Owner->Parent->Handle, Surface->Handle, &capabilities);

    SurfaceSize = {
        capabilities.currentExtent.width,
        capabilities.currentExtent.height
    };

    if(capabilities.maxImageCount == 1)
        return Result::Unsupported;
    if(capabilities.minImageCount > MaxSwapchainImages)
        return Result::Overflow;

    

    LogInfo("Vulkan: Swapchain: Requrest % Images %",capabilities.minImageCount, SurfaceSize);
    VkSwapchainCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.clipped = true;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.surface = Surface->Handle;
    info.oldSwapchain = VK_NULL_HANDLE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.preTransform = capabilities.currentTransform;
    info.presentMode = BestMode(Owner, *Surface);
    info.minImageCount = capabilities.minImageCount;
    info.imageArrayLayers = 1;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.imageExtent = {SurfaceSize.x, SurfaceSize.y};
    info.imageFormat = Format;
    info.imageColorSpace = Colorspace;

    if(vkCreateSwapchainKHR(Owner->Handle,&info,nullptr,&Handle) != VK_SUCCESS)
        return Result::Failure;

    return Result::Success;
}

Result Swapchain::CreateImageViews(){
    vkGetSwapchainImagesKHR(Owner->Handle, Handle, &ImagesCount, nullptr);
    LogInfo("Vulkan: Swapchain: Got % images",ImagesCount);
    if(ImagesCount > MaxSwapchainImages)
        return Result::Overflow;
    if(vkGetSwapchainImagesKHR(Owner->Handle, Handle, &ImagesCount, Images) != VK_SUCCESS)
        return Result::Unavailable;

    VkImageViewCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = Format;
    info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.layerCount = 1;
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

    Memory::Set(ImageViews,0,sizeof(ImageViews));

    for(int i = 0; i<ImagesCount; i++){
        info.image = Images[i];
        if(vkCreateImageView(Owner->Handle, &info, nullptr, &ImageViews[i]) != VK_SUCCESS)
            return Result::Failure;
    }
    return Result::Success;
}

void Swapchain::AcquireNext(const Vk::Semaphore &signal_semaphore){
    vkAcquireNextImageKHR(Owner->Handle, Handle, 0, signal_semaphore.Handle, VK_NULL_HANDLE, &CurrentImage);
}

void Swapchain::PresentCurrent(const ArrayPtr<VkSemaphore> &wait_semaphores){
    VkResult result;

    VkPresentInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pNext = nullptr;
    info.swapchainCount = 1;
    info.pSwapchains = &Handle;
    info.pImageIndices = &CurrentImage;
    info.pResults = &result;
    info.waitSemaphoreCount = wait_semaphores.Size;
    info.pWaitSemaphores = wait_semaphores.Pointer;

    vkQueuePresentKHR(Owner->GraphicsQueue.Handle, &info);
}

void Swapchain::DestroyImageViews(){
    for(int i = 0; i<ImagesCount; i++){
        if(ImageViews[i] != VK_NULL_HANDLE)
            vkDestroyImageView(Owner->Handle, ImageViews[i], nullptr);
    }
    Memory::Set(ImageViews,0,sizeof(ImageViews));
}

void Swapchain::DestroyChain(){
    vkDestroySwapchainKHR(Owner->Handle, Handle, nullptr);
}


void Swapchain::Destroy(){
    DestroyImageViews();
    DestroyChain();
}

};//namespace Vk::
};//namespace StraitX::

