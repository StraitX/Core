#ifndef STRAITX_VULKAN_COMMAND_BUFFER_IMPL_HPP
#define STRAITX_VULKAN_COMMAND_BUFFER_IMPL_HPP

#include "core/os/vulkan.hpp"
#include "graphics/api/command_buffer.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"

namespace Vk{

class CommandPoolImpl: public CommandPool{
private:
    VkCommandPool m_Handle = VK_NULL_HANDLE;
    QueueFamily::Type m_TargetQueueFamily;
    u32 m_TargetQueueFamilyIndex = -1;
public:
    CommandPoolImpl(QueueFamily::Type target_queue_family = QueueFamily::Type::Graphics);

    ~CommandPoolImpl();

    CommandBuffer *Alloc()override;

    void Free(CommandBuffer *buffer)override;
};

class CommandBufferImpl: public CommandBuffer{
private:
    VkCommandPool m_Pool = VK_NULL_HANDLE;
    VkCommandBuffer m_Handle = VK_NULL_HANDLE;
public:
    CommandBufferImpl(VkCommandPool pool);

    ~CommandBufferImpl();

    void Begin()override;

    void End()override;

    void Reset()override;
};

}//namespace Vk::

#endif//STRAITX_VULKAN_COMMAND_BUFFER_IMPL_HPP