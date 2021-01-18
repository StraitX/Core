#ifndef STRAITX_VULKAN_COMMAND_POOL_HPP
#define STRAITX_VULKAN_COMMAND_POOL_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "graphics/vulkan/queue.hpp"

namespace StraitX{
namespace Vk{

struct LogicalGPUImpl;

struct CommandPool{
    VkCommandPool Handle = VK_NULL_HANDLE;
    Vk::LogicalGPUImpl *Owner = nullptr;
    Vk::Queue Queue;

    Result Create(Vk::LogicalGPUImpl *owner, Vk::Queue queue);

    void Destroy();

};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_COMMAND_POOL_HPP