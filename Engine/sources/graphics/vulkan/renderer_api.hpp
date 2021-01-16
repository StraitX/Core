#ifndef STRAITX_VULKAN_RENDERER_HPP
#define STRAITX_VULKAN_RENDERER_HPP

#include "platform/vulkan.hpp"
#include "platform/vk_surface.hpp"
#include "platform/types.hpp"
#include "platform/result.hpp"
#include "platform/window.hpp"
#include "core/array_ptr.hpp"
#include "graphics/vulkan/graphics_api_impl.hpp"
#include "graphics/vulkan/physical_device.hpp"
#include "graphics/vulkan/logical_device.hpp"
#include "graphics/vulkan/swapchain.hpp"

namespace StraitX{
namespace Vk{

class RendererAPI{
public:
    Vk::GraphicsAPIImpl m_Instance;
    Vk::PhysicalDevice m_PhysicalDevice;
    Vk::LogicalDevice m_Device;
    Vk::Surface m_Surface;
    VkSurfaceFormatKHR m_Format = {VK_FORMAT_B8G8R8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR};
    Vk::Swapchain m_Swapchain;

    Result m_ErrInstance = Result::None;
    Result m_ErrDevice = Result::None;
    Result m_ErrSurface = Result::None;
    Result m_ErrSwapchain = Result::None;
public:
    static RendererAPI Instance;

    Result InitializeHardware();

    Result InitializeRender(const Window &window);

    Result FinalizeRender();

    Result FinalizeHardware();
private:
    Result PickBestPhysicalDevice();

};

};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_RENDERER_HPP
