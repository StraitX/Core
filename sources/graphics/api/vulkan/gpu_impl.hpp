#ifndef STRAITX_VULKAN_GPU_IMPL_HPP
#define STRAITX_VULKAN_GPU_IMPL_HPP

#include "graphics/api/gpu.hpp"
#include "core/os/vulkan.hpp"
#include "graphics/api/vulkan/queue.hpp"

namespace Vk{

class GPUImpl: public ::GPUImpl{
private:
    VkPhysicalDevice m_PhysicalHandle = VK_NULL_HANDLE;
    VkDevice m_Handle = VK_NULL_HANDLE;

    QueueProperties m_QueueProperties;

    VkQueue m_Queues[QueueFamily::FamilyCount] = {VK_NULL_HANDLE};
public:
    static GPUImpl s_Instance;

    Result Initialize();

    void Finalize();

    VkDevice Handle()const{
        return m_Handle;
    }

    VkPhysicalDevice PhysicalHandle()const{
        return m_PhysicalHandle;
    }

    VkQueue Queue(QueueFamily::Type type)const{
        return m_Queues[type];
    }

    u32 QueueIndex(QueueFamily::Type type)const{
        return m_QueueProperties.Family[type].Index;
    }
};

}//namespace Vk::

#endif//STRAITX_VULKAN_GPU_IMPL_HPP