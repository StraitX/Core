#include "core/log.hpp"
#include "core/os/memory.hpp"
#include "graphics/api/vulkan/graphics_api_impl.hpp"
#include "graphics/api/vulkan/swapchain_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"

namespace Vk{

bool IsSupported(VkPhysicalDevice dev, VkSurfaceKHR surface, VkFormat format, VkColorSpaceKHR color_space){
    u32 count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &count, nullptr);

    //auto *formats = (VkSurfaceFormatKHR*)alloca(count * sizeof(VkSurfaceFormatKHR));
    auto *formats = SX_STACK_ARRAY_ALLOC(VkSurfaceFormatKHR, count);

    vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &count, formats);

    bool support = false;

    for(u32 i = 0; i<count; i++){
        if(formats[i].colorSpace == color_space && formats[i].format == format)
            support = true;
    }

    return support;
}    

SwapchainImpl::SwapchainImpl(const Window *window):
    m_SurfaceWindow(window),
    m_TargetQueueFamily(QueueFamily::Graphics),
    m_TargetQueue(GPUImpl::s_Instance.Queue(m_TargetQueueFamily)),
    m_TargetQueueIndex(GPUImpl::s_Instance.QueueIndex(m_TargetQueueFamily))
{
    SX_CORE_CALL_ASSERT(
        m_Surface.Create(Vk::GraphicsAPIBackendImpl::s_Instance.Instance(), m_SurfaceWindow->Impl()), 
        Result::Success, 
        "Vk: Swapchain: Can't obtain surface"
    );
    
    {
        VkBool32 supported;
        vkGetPhysicalDeviceSurfaceSupportKHR(GPUImpl::s_Instance.PhysicalHandle(), m_TargetQueueIndex, m_Surface.Handle, &supported);
        if(!supported){
            LogError("Vk: Swapchain: Current Physical Device does not support swapchain");
            return;
        }
    }
    
    VkSurfaceCapabilitiesKHR capabilities;
    SX_VK_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(GPUImpl::s_Instance.PhysicalHandle(), m_Surface.Handle, &capabilities), "Vk: Swapchain: can't obtain surface sapabilites");

    if(capabilities.minImageCount > m_ImagesCount){
        LogWarn("Vk: Swapchain: System requires % framebuffers", capabilities.minImageCount);
        m_ImagesCount = capabilities.minImageCount;
    }

    SX_CORE_ASSERT((!capabilities.maxImageCount || m_ImagesCount <= capabilities.maxImageCount), "Vk: Swapchain: current system does not support this amount of framebuffers");
    
    if(!IsSupported(GPUImpl::s_Instance.PhysicalHandle(), m_Surface.Handle, m_Format, m_Colorspace)){
        LogError("Vk: Swapchain: ColorSpace and Format are not supported");
        return;
    }

    m_Size.x = capabilities.currentExtent.width;
    m_Size.y = capabilities.currentExtent.height;
    
    VkSwapchainCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.oldSwapchain = VK_NULL_HANDLE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.clipped = true;
    info.minImageCount = m_ImagesCount;
    info.preTransform = capabilities.currentTransform;
    info.surface = m_Surface.Handle;
    info.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR; // should be supported everywhere // TODO replace with VSync mode
    info.imageFormat = m_Format;
    info.imageColorSpace = m_Colorspace;
    info.imageExtent = capabilities.currentExtent;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // XXX For now we support just a single queue
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    SX_VK_ASSERT(vkCreateSwapchainKHR(GPUImpl::s_Instance, &info, nullptr, &m_Handle), "Vk: Swapchain: Can't create a swapchain");

    vkGetSwapchainImagesKHR(GPUImpl::s_Instance, m_Handle, &m_ImagesCount, nullptr);
    VkImage *images = SX_STACK_ARRAY_ALLOC(VkImage, m_ImagesCount);
    SX_VK_ASSERT(vkGetSwapchainImagesKHR(GPUImpl::s_Instance, m_Handle, &m_ImagesCount, images), "Vk: Swapchain: can't get images");

    for(u32 i = 0; i<m_ImagesCount; i++)
        m_Images.Emplace(images[i], m_Size.x, m_Size.y, TextureFormat::BGRA8, TextureUsageBits::TransferDst, TextureLayout::Undefined);

    for(u32 i = 0; i<m_ImagesCount; i++)
        m_ImagesPointers.Push(&m_Images[i]);

}

SwapchainImpl::~SwapchainImpl(){
    vkQueueWaitIdle(m_TargetQueue);

    m_ImagesPointers.Clear();
    m_Images.Clear();

    vkDestroySwapchainKHR(GPUImpl::s_Instance, m_Handle, nullptr);

    m_Surface.Destroy();
}

void SwapchainImpl::PresentCurrent(const Semaphore &wait_semaphore){
    VkResult result;

    VkSemaphore wait_semaphore_handle = (VkSemaphore)wait_semaphore.Handle();

    VkPresentInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pNext = nullptr;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &wait_semaphore_handle;
    info.swapchainCount = 1;
    info.pSwapchains = &m_Handle;
    info.pImageIndices = &m_CurrentImage;
    info.pResults = &result;

    vkQueuePresentKHR(m_TargetQueue, &info);
}

void SwapchainImpl::AcquireNext(const Semaphore &signal_semaphore, const Fence &signal_fence){
    VkSemaphore signal_semaphore_handle = (VkSemaphore)signal_semaphore.Handle();
    VkFence signal_fence_handle = (VkFence)signal_fence.Handle();

    vkAcquireNextImageKHR(GPUImpl::s_Instance, m_Handle, 0, signal_semaphore_handle, signal_fence_handle, &m_CurrentImage);
}

ConstSpan<Texture2D *> SwapchainImpl::Images()const{
    return m_ImagesPointers;
}

u32 SwapchainImpl::Current()const{
    return m_CurrentImage;
}

}//namespace Vk::