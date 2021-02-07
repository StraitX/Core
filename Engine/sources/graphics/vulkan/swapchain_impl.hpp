#ifndef STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP
#define STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP

#include "platform/vulkan.hpp"
#include "platform/vk_surface.hpp"
#include "core/math/vector2.hpp"
#include "graphics/api/swapchain.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/vulkan/fence.hpp"

namespace StraitX{
namespace Vk{

class SwapchainImpl: public Swapchain{
private:
    Vk::LogicalGPUImpl *m_Owner = nullptr;
    Vk::Surface m_Surface;
    Vector2u m_Size = {0, 0}; 

    VkFormat m_Format = VK_FORMAT_UNDEFINED;
    VkColorSpaceKHR m_Colorspace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

    VkSwapchainKHR m_Handle = VK_NULL_HANDLE; 

    Vk::Fence m_AcquireFence;
    u32 m_CurrentImage = 0;

public:
    SwapchainImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props);

    ~SwapchainImpl();

    virtual void SwapFramebuffers(GPUContext &context)override;

    static Swapchain *NewImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props);

    static void DeleteImpl(Swapchain *swapchain);
private:

    void PresentCurrent();

    void AcquireNext();
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP