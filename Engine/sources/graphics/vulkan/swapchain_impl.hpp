#ifndef STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP
#define STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP

#include "platform/vulkan.hpp"
#include "platform/vulkan_surface.hpp"
#include "core/math/vector2.hpp"
#include "core/push_array.hpp"
#include "graphics/api/swapchain.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/vulkan/fence.hpp"
#include "graphics/vulkan/framebuffer_impl.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"

namespace StraitX{
namespace Vk{

class SwapchainImpl: public Swapchain{
private:
    Vk::LogicalGPUImpl *m_Owner = nullptr;
    VulkanSurface m_Surface;
    Vector2u m_Size = {0, 0}; 

    VkFormat m_Format = VK_FORMAT_UNDEFINED;
    VkColorSpaceKHR m_Colorspace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

    VkSwapchainKHR m_Handle = VK_NULL_HANDLE; 

    Vk::Fence m_AcquireFence;
    u32 m_CurrentImage = 0;
    u32 m_ImagesCount = 0;

    Vk::RenderPassImpl m_FramebufferPass;
    PushArray<GPUTexture, MaxFramebuffers> m_Images;
    PushArray<Vk::FramebufferImpl, MaxFramebuffers> m_Framebuffers;
public:
    SwapchainImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props);

    ~SwapchainImpl();

    virtual const RenderPass *FramebufferPass()override;

    virtual const Framebuffer *CurrentFramebuffer()override;

    static Swapchain *NewImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props);

    static void DeleteImpl(Swapchain *swapchain);
private:
    void InitializeFramebuffers(GPUTexture::Format images_format);

    void FinalizeFramebuffers();
public:
    void PresentCurrent(VkSemaphore wait_semaphore);

    void AcquireNext(VkSemaphore signal_semaphore);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP