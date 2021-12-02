#ifndef STRAITX_VULKAN_GPU_IMPL_HPP
#define STRAITX_VULKAN_GPU_IMPL_HPP

#include "graphics/api/gpu.hpp"
#include "core/os/vulkan.hpp"
#include "core/list.hpp"
#include "core/pair.hpp"
#include "graphics/api/vulkan/queue.hpp"
#include "graphics/api/vulkan/memory.hpp"

namespace Vk{

class GPUImpl: public ::GPUImpl{
private:
    VkPhysicalDevice m_PhysicalHandle = VK_NULL_HANDLE;
    VkDevice m_Handle = VK_NULL_HANDLE;

    QueueProperties m_QueueProperties;
    MemoryProperties m_MemoryProperties;

    AbstractQueue m_Queues[QueueFamily::FamilyCount] = {};

    CommandPool *m_SyncCommandBufferPool = nullptr;
    List<Pair<CommandBuffer*, Fence>> m_SyncPairs;
public:
    static GPUImpl s_Instance;

    Result Initialize()override;

    void Finalize()override;

    void Execute(CommandBuffer *buffer, Span<u64> wait_semaphore_handles, Span<u64> signal_semaphore_handles, const Fence &signal_fence)override;

    void SyncSemaphores(ConstSpan<Semaphore> wait, ConstSpan<Semaphore> signal)override;

    VkDevice Handle()const{
        return m_Handle;
    }

    operator VkDevice()const{
        return Handle();
    }

    VkPhysicalDevice PhysicalHandle()const{
        return m_PhysicalHandle;
    }

    VkQueue Queue(QueueFamily::Type type)const{
        return m_Queues[type].Handle;
    }

    u32 QueueIndex(QueueFamily::Type type)const{
        return m_Queues[type].BackingQueueFamily->Index;
    }

    u32 MemoryIndex(MemoryType::Type type)const{
        return m_MemoryProperties.AbstractMemoryTypes[(size_t)type].Index;
    }

    MemoryType::Type BackingMemoryType(MemoryType::Type type)const{
        return m_MemoryProperties.AbstractMemoryTypes[type].BackingMemoryType;
    }
};

}//namespace Vk::

#endif//STRAITX_VULKAN_GPU_IMPL_HPP