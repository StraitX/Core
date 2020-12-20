#ifndef STRAITX_VULKAN_COMMAND_POOL_HPP
#define STRAITX_VULKAN_COMMAND_POOL_HPP

#include "platform/vulkan.hpp"
#include "platform/error.hpp"
#include "graphics/vulkan/queue.hpp"
#include "graphics/vulkan/logical_device.hpp"

namespace StraitX{
namespace Vk{

struct CommandPool{
    VkCommandPool Handle = VK_NULL_HANDLE;
    Vk::LogicalDevice *Owner = nullptr;
    Vk::Queue Queue;

    Error Create(Vk::LogicalDevice *owner, Vk::Queue queue);

    void Destroy();

};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_COMMAND_POOL_HPP