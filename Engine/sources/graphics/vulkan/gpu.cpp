#include "platform/memory.hpp"
#include "core/log.hpp"
#include "core/assert.hpp"
#include "core/span.hpp"
#include "core/string.hpp"
#include "core/push_array.hpp"
#include "graphics/vulkan/gpu.hpp"
#include "graphics/vulkan/cpu_buffer_impl.hpp"

namespace Vk{

GPU GPU::s_Instance;

Result GPU::Initialize(VkPhysicalDevice device){
    m_PhysicalHandle = device;

    m_MemoryProperties = MemoryProperties::Get(m_PhysicalHandle);
    m_QueueProperties = QueueProperties::Get(m_PhysicalHandle);

    if(m_MemoryProperties.Layout == MemoryLayout::Unknown){
        LogError("Vk: GPU: Unknown Memory Layout");
        return Result::Unsupported;
    }

    if(m_QueueProperties.Family[QueueFamily::Graphics].Index == InvalidIndex){
        LogError("Vk: GPU: this gpu does not support graphics queue");
        return Result::Unsupported;
    }
    
    PushArray<VkDeviceQueueCreateInfo, QueueFamily::FamilyCount> queues;

    float priority = 1.0;

    for(int i = 0; i<QueueFamily::FamilyCount; i++){
        if(m_QueueProperties.Family[i].Index != InvalidIndex){
            VkDeviceQueueCreateInfo qinfo;
            qinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            qinfo.pNext = 0;
            qinfo.flags = 0;
            qinfo.queueFamilyIndex = m_QueueProperties.Family[i].Index;
            qinfo.queueCount = 1;
            qinfo.pQueuePriorities = &priority;

            queues.Push(qinfo);
        }
    }

    const char *ext[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkPhysicalDeviceFeatures features; /// XXX
    vkGetPhysicalDeviceFeatures(m_PhysicalHandle, &features);

    VkDeviceCreateInfo info;
    info.sType                  = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
    info.enabledExtensionCount  = lengthof(ext);
    info.ppEnabledExtensionNames= ext;
    info.enabledLayerCount      = 0;
    info.ppEnabledLayerNames    = nullptr;
    info.pEnabledFeatures       = &features;
    info.queueCreateInfoCount   = queues.Size();
    info.pQueueCreateInfos      = queues.Data();

    LogInfo("Vk: GPU: Creating device with % queues", info.queueCreateInfoCount);

    if(vkCreateDevice(m_PhysicalHandle, &info, nullptr, &m_Handle) != VK_SUCCESS)
        return Result::NullObject;

    for(int i = 0; i<QueueFamily::FamilyCount; i++){
        if(m_QueueProperties.Family[i].Index != InvalidIndex)
            vkGetDeviceQueue(m_Handle, m_QueueProperties.Family[i].Index, 0, &m_Queues[i]);
    }

    for(int i = 1; i<QueueFamily::FamilyCount; i++)
        if(m_Queues[i] == VK_NULL_HANDLE)
            m_Queues[i] = m_Queues[i - 1];
    
    SX_ASSERT(m_Queues[QueueFamily::Graphics] != VK_NULL_HANDLE);
    SX_ASSERT(m_Queues[QueueFamily::Compute] != VK_NULL_HANDLE);
    SX_ASSERT(m_Queues[QueueFamily::Transfer] != VK_NULL_HANDLE);

    SX_ASSERT(m_MemoryProperties.Memory[MemoryType::VRAM].Index != InvalidIndex);
    SX_ASSERT(m_MemoryProperties.Memory[MemoryType::DynamicVRAM].Index != InvalidIndex);
    SX_ASSERT(m_MemoryProperties.Memory[MemoryType::RAM].Index != InvalidIndex);
    SX_ASSERT(m_MemoryProperties.Memory[MemoryType::UncachedRAM].Index != InvalidIndex);
    
    return Result::Success;
}

void GPU::Finalize(){
    vkDeviceWaitIdle(m_Handle);
    
    vkDestroyDevice(m_Handle,nullptr);
}

}//namespace Vk::