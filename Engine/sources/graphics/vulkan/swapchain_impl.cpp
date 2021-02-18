#include <new>
#include "platform/memory.hpp"
#include "core/assert.hpp"
#include "core/log.hpp"
#include "graphics/vulkan/graphics_api_impl.hpp"
#include "graphics/vulkan/swapchain_impl.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"

namespace StraitX{
namespace Vk{

constexpr VkColorSpaceKHR DesiredColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;


static bool IsSupported(VkPhysicalDevice dev, VkSurfaceKHR surface, VkFormat format, VkColorSpaceKHR color_space){
    u32 count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &count, nullptr);

    auto *formats = (VkSurfaceFormatKHR*)alloca(count * sizeof(VkSurfaceFormatKHR));

    vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &count, formats);

    bool support = false;

    for(u32 i = 0; i<count; i++){
        if(formats[i].colorSpace == color_space && formats[i].format == format)
            support = true;
    }

    return support;
}


SwapchainImpl::SwapchainImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props):
    m_Owner(static_cast<Vk::LogicalGPUImpl *>(&gpu)),
    m_Colorspace(DesiredColorSpace),
    m_Format(GPUTextureImpl::s_FormatTable[(size_t)props.ColorAttachmentDescription.Format]),
    m_AcquireFence(m_Owner->Handle),
    m_FramebufferPass(gpu, {{&props.ColorAttachmentDescription, 1}})
{
    CoreFunctionAssert(m_Surface.Create(Vk::GraphicsAPIImpl::Instance.Handle, window),Result::Success, "Vk: SwapchainImpl: Can't obtain surface");
    
    VkBool32 supported;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_Owner->PhysicalHandle, m_Owner->GraphicsQueue.FamilyIndex, m_Surface.Handle, &supported);
    if(!supported){
        LogError("Vk: SwapchainImpl: Current Physical Device does not support swapchain");
        return;
    }
    VkSurfaceCapabilitiesKHR capabilities;
    CoreFunctionAssert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Owner->PhysicalHandle, m_Surface.Handle, &capabilities), VK_SUCCESS, "Vk: SwapchainImpl: can't obtain surface sapabilites");

    CoreAssert(props.FramebuffersCount >= capabilities.minImageCount && capabilities.maxImageCount && props.FramebuffersCount <= capabilities.maxImageCount, "Vk: SwapchainImpl: current system does not support this amount of framebuffers");
    
    if(!IsSupported(m_Owner->PhysicalHandle, m_Surface.Handle, m_Format, m_Colorspace)){
        LogError("Vk: SwapchainImpl: ColorSpace and Format are not supported");
        return;
    }
    
    VkSwapchainCreateInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.pNext = nullptr;
    info.flags = 0;
    info.oldSwapchain = VK_NULL_HANDLE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.clipped = true;
    info.minImageCount = props.FramebuffersCount;
    info.preTransform = capabilities.currentTransform;
    info.surface = m_Surface.Handle;
    info.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR; // should be supported everywhere // TODO replace with VSync mode
    info.imageFormat = m_Format;
    info.imageColorSpace = m_Colorspace;
    info.imageExtent = capabilities.currentExtent;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // XXX For now we support just a single queue
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    CoreFunctionAssert(vkCreateSwapchainKHR(m_Owner->Handle, &info, nullptr, &m_Handle), VK_SUCCESS, "Vk: SwapchainImpl: Can't create a swapchain");

    InitializeFramebuffers(props.ColorAttachmentDescription.Format);

    AcquireNext();
}

SwapchainImpl::~SwapchainImpl(){
    vkQueueWaitIdle(m_Owner->GraphicsQueue.Handle);

    FinalizeFramebuffers();
    
    vkDestroySwapchainKHR(m_Owner->Handle, m_Handle, nullptr);

    m_Surface.Destroy();
}

void SwapchainImpl::SwapFramebuffers(GPUContext &context){
    PresentCurrent();
    AcquireNext();
}

const RenderPass *SwapchainImpl::FramebufferPass(){
    return &m_FramebufferPass;
}

const Framebuffer *SwapchainImpl::CurrentFramebuffer(){
    return &m_Framebuffers[m_CurrentImage];
}

Swapchain *SwapchainImpl::NewImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props){
    return new (Memory::Alloc(sizeof(Vk::SwapchainImpl))) SwapchainImpl(gpu, window, props);
}

void SwapchainImpl::DeleteImpl(Swapchain *swapchain){
    swapchain->~Swapchain();
    Memory::Free(swapchain);
}

void SwapchainImpl::InitializeFramebuffers(GPUTexture::Format images_format){
    u32 images_count = 0;
    vkGetSwapchainImagesKHR(m_Owner->Handle, m_Handle, &images_count, nullptr);
    CoreAssert(images_count <= MaxFramebuffers, "Vk: Swapchain: unsupported amount of Images");

    auto *images = (VkImage*)alloca(images_count * sizeof(VkImage));

    vkGetSwapchainImagesKHR(m_Owner->Handle, m_Handle, &images_count, images);

    for(u32 i = 0; i<images_count; ++i){
        GPUTexture texture;

        GPUTextureImpl impl(texture);
        impl.CreateFromChainImage(images[i], images_format);

        m_Images.Push(Move(texture));
    }
}

void SwapchainImpl::FinalizeFramebuffers(){
    for(auto &image: m_Images){
        GPUTextureImpl impl(image);
        impl.DestroyFromChainImage();
    }
}

void SwapchainImpl::PresentCurrent(){
    VkResult result;

    VkPresentInfoKHR info;
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.pNext = nullptr;
    info.waitSemaphoreCount = 0;
    info.pWaitSemaphores = nullptr;
    info.swapchainCount = 1;
    info.pSwapchains = &m_Handle;
    info.pImageIndices = &m_CurrentImage;
    info.pResults = &result;

    vkQueuePresentKHR(m_Owner->GraphicsQueue.Handle, &info);
}

void SwapchainImpl::AcquireNext(){
    vkAcquireNextImageKHR(m_Owner->Handle, m_Handle, 0, VK_NULL_HANDLE, m_AcquireFence.Handle, &m_CurrentImage);

    m_AcquireFence.WaitFor();/// TODO Get rid of this // Swapchain is Immediate mode for now :'-(
}

}//namespace Vk::
}//namespace StraitX::
