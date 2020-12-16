#ifndef STRAITX_VULKAN_RENDERER_HPP
#define STRAITX_VULKAN_RENDERER_HPP

#include "platform/vulkan.hpp"
#include "platform/types.hpp"
#include "platform/error.hpp"
#include "platform/window.hpp"
#include "core/array_ptr.hpp"
#include "core/memory/stack_allocator.hpp"
#include "graphics/vulkan/instance.hpp"
#include "graphics/vulkan/physical_device.hpp"
#include "graphics/vulkan/logical_device.hpp"

namespace StraitX{
namespace Vk{

constexpr size_t MaxExpectedDevices = 4;

class RendererAPI{
private:
    StackAllocator m_Allocator = {(sizeof(VkPhysicalDevice)+sizeof(Vk::PhysicalDevice)) * MaxExpectedDevices};
    Vk::Instance m_Instance;
    ArrayPtr<Vk::PhysicalDevice> m_PhysicalDeivces;
    Vk::LogicalDevice m_Device;
public:
    static RendererAPI Instance;

    Error InitializeHardware();

    Error InitializeRender(const Window &window);

    Error FinalizeRender();

    Error FinalizeHardware();

};

};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_RENDERER_HPP