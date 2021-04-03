#include "platform/memory.hpp"
#include "core/log.hpp"
#include "core/assert.hpp"
#include "core/array_ptr.hpp"
#include "core/string.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/vulkan/cpu_buffer_impl.hpp"

namespace StraitX{
namespace Vk{

LogicalGPUImpl LogicalGPUImpl::Instance;

Result LogicalGPUImpl::Initialize(const PhysicalGPU &gpu){
    { //First Stage Init
        PhysicalHandle = reinterpret_cast<VkPhysicalDevice>(gpu.Handle.U64);
        Vendor         = gpu.Vendor;
        Type           = gpu.Type;

        QueryQueues();
        Memory.Query(PhysicalHandle);
    }
    
    { // Device creation
        const char *ext[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        VkDeviceQueueCreateInfo qinfo[2];
        //GeneralQueue
        float gprior = 1.0;
        qinfo[0].sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qinfo[0].pNext              = nullptr;
        qinfo[0].flags              = 0;
        qinfo[0].queueFamilyIndex   = GeneralQueueFamily;
        qinfo[0].queueCount         = 1;
        qinfo[0].pQueuePriorities   = &gprior;

        qinfo[1].sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qinfo[1].pNext              = nullptr;
        qinfo[1].flags              = 0;
        qinfo[1].queueFamilyIndex   = TransferQueueFamily;
        qinfo[1].queueCount         = 1;
        qinfo[1].pQueuePriorities   = &gprior;

        VkPhysicalDeviceFeatures features; /// XXX
        vkGetPhysicalDeviceFeatures(PhysicalHandle, &features);

        VkDeviceCreateInfo info;
        info.sType                  = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        info.pNext                  = nullptr;
        info.flags                  = 0;
        info.enabledExtensionCount  = lengthof(ext);
        info.ppEnabledExtensionNames= ext;
        info.enabledLayerCount      = 0;
        info.ppEnabledLayerNames    = nullptr;
        info.pEnabledFeatures       = &features;
        info.queueCreateInfoCount   = (TransferQueueFamily != InvalidIndex ) ? 2 : 1;
        info.pQueueCreateInfos      = qinfo;

        LogInfo("Vulkan: Creating device with % queues", info.queueCreateInfoCount);

        if(vkCreateDevice(PhysicalHandle,&info,nullptr,&Handle) != VK_SUCCESS)
            return Result::NullObject;
    }

    Allocator.Initialize(this);

    GeneralQueue.Obtain(Handle, GeneralQueueFamily, 0);
    CoreAssert(GeneralQueue.IsInitialized(), "LogicalDevice: can't retrieve GeneralQueue handle");

    if(TransferQueueFamily != InvalidIndex){
        TransferQueue.Obtain(Handle, TransferQueueFamily, 0);
        CoreAssert(GeneralQueue.IsInitialized(), "LogicalDevice: can't retrieve TransferQueue handle");
    }else{
        TransferQueue = GeneralQueue;
    }

    VkCommandPoolCreateInfo pool_info;
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.pNext = nullptr;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = GeneralQueue.FamilyIndex;

    CoreFunctionAssert(vkCreateCommandPool(Handle, &pool_info, nullptr, &Pool), VK_SUCCESS, "Vk: LogicalGPU: Failed to create command pool");

    VkCommandBufferAllocateInfo buffer_info;
    buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    buffer_info.pNext = nullptr;
    buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    buffer_info.commandPool = Pool;
    buffer_info.commandBufferCount = 1;

    CoreFunctionAssert(vkAllocateCommandBuffers(Handle, &buffer_info, &TransferCmdBuffer), VK_SUCCESS, "Vk: LogicalGPU: Failed to allocate command buffer");

    return Result::Success;
}

void LogicalGPUImpl::Finalize(){
    vkDeviceWaitIdle(Handle);

    Allocator.Finalize();
    
    vkDestroyDevice(Handle,nullptr);
}

sx_inline bool IsGeneral(VkQueueFlags flags){
    return flags & VK_QUEUE_GRAPHICS_BIT
    &&     flags & VK_QUEUE_COMPUTE_BIT
    &&     flags & VK_QUEUE_TRANSFER_BIT;
}

// Most probably a DMA engine queue
sx_inline bool IsTransfer(VkQueueFlags flags){
    return !(flags & VK_QUEUE_GRAPHICS_BIT)
    &&     !(flags & VK_QUEUE_COMPUTE_BIT)
    &&       flags & VK_QUEUE_TRANSFER_BIT;
}

void LogicalGPUImpl::QueryQueues(){
    u32 props_count = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(PhysicalHandle, &props_count, nullptr);
    VkQueueFamilyProperties *props = (VkQueueFamilyProperties*)alloca(props_count*sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(PhysicalHandle,&props_count,props);

    int i = 0;
    for(; i<props_count; i++){
        if(IsGeneral(props[i].queueFlags))
            GeneralQueueFamily = i;
        if(IsTransfer(props[i].queueFlags))
            TransferQueueFamily = i;
    }

    DLogInfo("Vulkan: GeneralQueue   %",GeneralQueueFamily != InvalidIndex);
    DLogInfo("Vulkan: TransferQueue  %",TransferQueueFamily != InvalidIndex);
}

}//namespace Vk::
}//namespace StraitX::