#ifndef STRAITX_VULKAN_LOGICAL_DEVICE_HPP
#define STRAITX_VULKAN_LOGICAL_DEVICE_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "core/array_ptr.hpp"
#include "graphics/vulkan/physical_device.hpp"
#include "graphics/vulkan/queue.hpp"

namespace StraitX{
namespace Vk{

struct LogicalDevice{
    VkDevice Handle = VK_NULL_HANDLE;
    Vk::PhysicalDevice *Parent = nullptr;
    
    Vk::Queue GraphicsQueue;

    Result Create(PhysicalDevice *parent, const ArrayPtr<const char*> &extensions, const ArrayPtr<const char*> &layers);


    void Destroy();

};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_LOGICAL_DEVICE_HPP