#ifndef STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP
#define STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP

#include "platform/vulkan.hpp"
#include "platform/vk_surface.hpp"
#include "core/math/vector2.hpp"
#include "graphics/api/swapchain.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

class SwapchainImpl: public Swapchain{
private:
    Vk::LogicalGPUImpl *m_Owner = nullptr;
    Vk::Surface m_Surface;
    Vector2u m_Size = {0, 0}; 
    VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE; 
public:
    SwapchainImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props);

    ~SwapchainImpl();

    virtual void SwapFramebuffers(GPUContext &context)override;

    static Swapchain *NewImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props);

    static void DeleteImpl(Swapchain *swapchain);

};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_SWAPCHAIN_IMPL_HPP