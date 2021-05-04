#ifndef STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP
#define STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP

#include "platform/vulkan.hpp"
#include "platform/vulkan_surface.hpp"
#include "core/math/vector2.hpp"
#include "core/push_array.hpp"
#include "graphics/api/swapchain.hpp"
#include "graphics/vulkan/fence.hpp"
#include "graphics/vulkan/framebuffer_impl.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/queue.hpp"

namespace StraitX{
namespace Vk{

class SwapchainImpl: public Swapchain{
private:
    VulkanSurface m_Surface;
    Vector2u m_Size = {0, 0}; 

    VkFormat m_Format = VK_FORMAT_B8G8R8A8_UNORM;
    VkColorSpaceKHR m_Colorspace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

    VkSwapchainKHR m_Handle = VK_NULL_HANDLE; 

    Vk::Fence m_AcquireFence;
    u32 m_CurrentImage = 0;
    u32 m_ImagesCount = 0;

    Vk::RenderPassImpl m_FramebufferPass;
    PushArray<GPUTexture, MaxFramebuffers> m_Images;
    PushArray<Vk::FramebufferImpl, MaxFramebuffers> m_Framebuffers;

    QueueFamily::Type m_TargetQueueFamily;
    VkQueue m_TargetQueue = VK_NULL_HANDLE;
    u32 m_TargetQueueIndex = InvalidIndex;
public:
    SwapchainImpl(const Window &window, const SwapchainProperties &props);

    ~SwapchainImpl();

    virtual const Framebuffer *CurrentFramebuffer()override;

    static Swapchain *NewImpl(const Window &window, const SwapchainProperties &props);

    static void DeleteImpl(Swapchain *swapchain);
private:
    void InitializeFramebuffers(VkFormat images_format);

    void FinalizeFramebuffers();
public:
    void PresentCurrent(VkSemaphore wait_semaphore);

    void AcquireNext(VkSemaphore signal_semaphore);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP