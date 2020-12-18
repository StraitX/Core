#ifndef STRAITX_VULKAN_RENDERER_HPP
#define STRAITX_VULKAN_RENDERER_HPP

#include "platform/vulkan.hpp"
#include "platform/vk_surface.hpp"
#include "platform/types.hpp"
#include "platform/error.hpp"
#include "platform/window.hpp"
#include "core/array_ptr.hpp"
#include "core/memory/stack_allocator.hpp"
#include "graphics/vulkan/instance.hpp"
#include "graphics/vulkan/physical_device.hpp"
#include "graphics/vulkan/logical_device.hpp"
#include "graphics/vulkan/swapchain.hpp"

namespace StraitX{
namespace Vk{

constexpr size_t MaxExpectedDevices = 4;

class RendererAPI{
private:
    Vk::Instance m_Instance;
    Vk::PhysicalDevice m_PhysicalDevice;
    Vk::LogicalDevice m_Device;
    Vk::Surface m_Surface;
    VkSurfaceFormatKHR m_Format = {VK_FORMAT_B8G8R8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR};
    Vk::Swapchain m_Swapchain;

    Error m_ErrInstance = Error::None;
    Error m_ErrDevice = Error::None;
    Error m_ErrSurface = Error::None;
    Error m_ErrSwapchain = Error::None;
public:
    static RendererAPI Instance;

    Error InitializeHardware();

    Error InitializeRender(const Window &window);

    Error FinalizeRender();

    Error FinalizeHardware();
private:
    Error PickBestPhysicalDevice();

};

};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_RENDERER_HPP
