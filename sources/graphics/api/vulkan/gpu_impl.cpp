#include "core/os/memory.hpp"
#include "core/assert.hpp"
#include "core/log.hpp"
#include "core/fixed_list.hpp"
#include "graphics/api/vulkan/graphics_api_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "graphics/api/vulkan/command_buffer_impl.hpp"
#include "graphics/api/vulkan/immediate.hpp"

namespace Vk{

GPUType VkTypeToGPUType(u32 type){
    switch (type) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:      return GPUType::Discrete;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:    return GPUType::Integrated;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:       return GPUType::Virtual;
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:             return GPUType::Unknown;
    default:
        return GPUType::Unknown;
    }
}

int GetPriority(GPUType type){
    switch (type) {
    case GPUType::Unknown: return 0;
    case GPUType::Virtual: return 1;
    case GPUType::Integrated: return 2;
    case GPUType::Discrete: return 3;
    }
}

VkPhysicalDevice PickBestPhysicalDevice(VkInstance instance){
    u32 count = 0;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);

    auto *devices = SX_STACK_ARRAY_ALLOC(VkPhysicalDevice, count);

    vkEnumeratePhysicalDevices(instance, &count, devices);

    VkPhysicalDevice best = devices[0];
    int best_score = 0;
    
    for(u32 i = 0; i<count; i++){
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(devices[i], &props);

        u32 families_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &families_count, nullptr);

        int score = GetPriority(VkTypeToGPUType(props.deviceType))*10 + families_count * 5; 

        if(score > best_score){
            best = devices[i];
            best_score = score;
        }
    }

    return best;
}

GPUImpl GPUImpl::s_Instance;

Result GPUImpl::Initialize(){
    m_PhysicalHandle = PickBestPhysicalDevice(GraphicsAPIBackendImpl::s_Instance.Instance());

    m_MemoryProperties = MemoryProperties::Get(m_PhysicalHandle);
    m_QueueProperties = QueueProperties::Get(m_PhysicalHandle);

    if(m_QueueProperties.Family[QueueFamily::Graphics].Index == InvalidIndex 
    || m_QueueProperties.Family[QueueFamily::Graphics].Count == 0){
        LogError("Vk: GPU: this gpu does not support graphics queue");
        return Result::Unsupported;
    }
    
    FixedList<VkDeviceQueueCreateInfo, QueueFamily::FamilyCount> queue_creations_infos;

    float priority = 1.0;

    for(int i = 0; i<QueueFamily::FamilyCount; i++){
        if(m_QueueProperties.Family[i].Index != InvalidIndex && m_QueueProperties.Family[i].Count){
            VkDeviceQueueCreateInfo qinfo;
            qinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            qinfo.pNext = 0;
            qinfo.flags = 0;
            qinfo.queueFamilyIndex = m_QueueProperties.Family[i].Index;
            qinfo.queueCount = 1;
            qinfo.pQueuePriorities = &priority;

            m_Queues[i].BackingQueueFamily = &m_QueueProperties.Family[i];
            queue_creations_infos.Add(qinfo);
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
    info.queueCreateInfoCount   = queue_creations_infos.Size();
    info.pQueueCreateInfos      = queue_creations_infos.Data();

    //LogInfo("Vk: GPU: Creating device with % queues", info.queueCreateInfoCount);

    if(vkCreateDevice(m_PhysicalHandle, &info, nullptr, &m_Handle) != VK_SUCCESS)
        return Result::NullObject;

    for(int i = 0; i<QueueFamily::FamilyCount; i++){
        if(m_Queues[i].BackingQueueFamily != nullptr){
            vkGetDeviceQueue(m_Handle, m_Queues[i].BackingQueueFamily->Index, 0, &m_Queues[i].Handle);
        }else{
            SX_CORE_ASSERT(i != 0, "Vk: QueueFallbackOperation: Graphics queue has no backing queue family");
            m_Queues[i] = m_Queues[i - 1];
        }
    }
    
    SX_ASSERT(m_Queues[QueueFamily::Graphics].Handle != VK_NULL_HANDLE);
    SX_ASSERT(m_Queues[QueueFamily::Compute ].Handle != VK_NULL_HANDLE);
    SX_ASSERT(m_Queues[QueueFamily::Transfer].Handle != VK_NULL_HANDLE);

    SX_ASSERT(m_MemoryProperties.AbstractMemoryTypes[MemoryType::VRAM].Index != InvalidIndex);
    SX_ASSERT(m_MemoryProperties.AbstractMemoryTypes[MemoryType::DynamicVRAM].Index != InvalidIndex);
    SX_ASSERT(m_MemoryProperties.AbstractMemoryTypes[MemoryType::RAM].Index != InvalidIndex);
    SX_ASSERT(m_MemoryProperties.AbstractMemoryTypes[MemoryType::UncachedRAM].Index != InvalidIndex);


    Immediate::Initialize();


    m_SyncCommandBufferPool = CommandPool::Create();
    return Result::Success;
}

void GPUImpl::Finalize(){
    delete m_SyncCommandBufferPool;

    Immediate::Finalize();
    vkDeviceWaitIdle(m_Handle);
    
    vkDestroyDevice(m_Handle,nullptr);
}

void GPUImpl::Execute(CommandBuffer *buffer, Span<u64> wait_semaphore_handles, Span<u64> signal_semaphore_handles, const Fence &signal_fence){
    Vk::CommandBufferImpl *buffer_impl = (Vk::CommandBufferImpl*)buffer;
    VkCommandBuffer buffer_handle = *buffer_impl;

    buffer_impl->OnExecute();

    auto *stages = SX_STACK_ARRAY_ALLOC(VkPipelineStageFlags, wait_semaphore_handles.Size());

    for(size_t i = 0; i<wait_semaphore_handles.Size(); ++i){
        stages[i] = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    }

    VkSubmitInfo info;
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pNext = nullptr;
    info.waitSemaphoreCount = wait_semaphore_handles.Size();
    info.pWaitSemaphores = (VkSemaphore*)wait_semaphore_handles.Pointer();
    info.pWaitDstStageMask = stages;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &buffer_handle;
    info.signalSemaphoreCount = signal_semaphore_handles.Size();
    info.pSignalSemaphores = (VkSemaphore*)signal_semaphore_handles.Pointer();

    vkQueueSubmit(GPUImpl::Queue(buffer_impl->Pool()->TargetQueueType()), 1, &info, (VkFence)signal_fence.Handle());
}

void GPUImpl::SyncSemaphores(ConstSpan<Semaphore> wait, ConstSpan<Semaphore> signal){
    Pair<CommandBuffer*, Fence> *sync = nullptr;
    for(auto &pair: m_SyncPairs){
        if(pair.Second.IsSignaled()){
            pair.Second.Reset();
            sync = &pair;
            break;
        }
    }

    if(sync == nullptr){
        CommandBuffer *buffer = m_SyncCommandBufferPool->Alloc();
        buffer->Begin();
        buffer->End();
        m_SyncPairs.Emplace(Move(buffer), Fence());
        sync = &m_SyncPairs.Last();
    }

    GPU::Execute(sync->First, wait, signal, sync->Second);
}

}//namespace Vk::