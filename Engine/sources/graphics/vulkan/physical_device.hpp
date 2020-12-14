#ifndef STRAITX_VULKAN_PHYSICAL_DEVICE_HPP
#define STRAITX_VULKAN_PHYSICAL_DEVICE_HPP

#include "platform/vulkan.hpp"
#include "platform/types.hpp"
#include "platform/error.hpp"

namespace StraitX{
namespace Vk{

enum class DeviceType: u8{
    Unknown,
    DiscreteGPU,
    IntegratedGPU
};

struct PhysicalDevice{
    VkPhysicalDevice Handle = VK_NULL_HANDLE;

    DeviceType Type = DeviceType::Unknown;

    u32 VRAM    = -1;
    u32 RAM     = -1;
    //optional
    u32 DynamicVRAM = -1;

    PhysicalDevice(VkPhysicalDevice device);

};

};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_PHYSICAL_DEVICE_HPP