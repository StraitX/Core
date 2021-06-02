#ifndef STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP
#define STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP

#include "platform/vulkan.hpp"
#include "core/span.hpp"
#include "graphics/vulkan/queue.hpp"
#include "graphics/vulkan/memory.hpp"
#include "graphics/vulkan/command_buffer.hpp"
#include "graphics/vulkan/fence.hpp"
#include "graphics/vulkan/queue.hpp"

namespace StraitX{
namespace Vk{

class GPU{
private:
    VkPhysicalDevice m_PhysicalHandle = VK_NULL_HANDLE;
    VkDevice  m_Handle = VK_NULL_HANDLE;

    QueueProperties m_QueueProperties;
    MemoryProperties m_MemoryProperties;

    VkQueue m_Queues[QueueFamily::FamilyCount] = {VK_NULL_HANDLE};
private:
    static GPU s_Instance;
public:

    Result Initialize(VkPhysicalDevice device);

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

    u32 MemoryIndex(MemoryType::Type type)const{
        return m_MemoryProperties.Memory[type].Index;
    }

    bool IsMappable(MemoryType::Type type)const{
        return m_MemoryProperties.IsMappable(type); 
    }

    static GPU &Get(){
        return s_Instance;
    }
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_LOGICAL_GPU_IMPL_HPP