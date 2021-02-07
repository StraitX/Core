#include <new>
#include "platform/memory.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/graphics_api_impl.hpp"
#include "graphics/vulkan/swapchain_impl.hpp"

namespace StraitX{
namespace Vk{

SwapchainImpl::SwapchainImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props):
    m_Owner(static_cast<Vk::LogicalGPUImpl *>(&gpu))
{
    CoreFunctionAssert(m_Surface.Create(Vk::GraphicsAPIImpl::Instance.Handle, window),Result::Success, "Vk: SwapchainImpl: Can't obtain surface");

    VkSurfaceCapabilitiesKHR capabilities;
    CoreFunctionAssert(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Owner->PhysicalHandle, m_Surface.Handle, &capabilities), VK_SUCCESS, "Vk: SwapchainImpl: can't obtain surface sapabilites");

    CoreAssert(props.FramebuffersCount >= capabilities.minImageCount && capabilities.maxImageCount && props.FramebuffersCount <= capabilities.maxImageCount, "Vk: SwapchainImpl: current system does not support this amount of framebuffers");
    //TODO Impl
}

SwapchainImpl::~SwapchainImpl(){
    //TODO Impl

    m_Surface.Destroy();
}

void SwapchainImpl::SwapFramebuffers(GPUContext &context){
    //TODO Impl
}

Swapchain *SwapchainImpl::NewImpl(LogicalGPU &gpu, const Window &window, const SwapchainProperties &props){
    return new (Memory::Alloc(sizeof(Vk::SwapchainImpl))) SwapchainImpl(gpu, window, props);
}

void SwapchainImpl::DeleteImpl(Swapchain *swapchain){
    swapchain->~Swapchain();
    Memory::Free(swapchain);
}

}//namespace Vk::
}//namespace StraitX::
