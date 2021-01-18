#include "core/log.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"


namespace StraitX{
namespace Vk{

Result LogicalGPUImpl::Create(const PhysicalGPU &gpu){
    { //First Stage Init
        PhysicalHandle = reinterpret_cast<VkPhysicalDevice>(gpu.Handle.U64);
        Vendor         = gpu.Vendor;
        Type           = gpu.Type;

        QueryQueues();
        Allocator.Initialize(this);

    }
    
    { // Device creation
        VkDeviceQueueCreateInfo qinfo[2];
        //GraphicsQueue
        float gprior = 1.0;
        qinfo[0].sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qinfo[0].pNext              = nullptr;
        qinfo[0].flags              = 0;
        qinfo[0].queueFamilyIndex   = GraphicsQueueFamily;
        qinfo[0].queueCount         = 1;
        qinfo[0].pQueuePriorities   = &gprior;

        //Optional Transfer Queue
        float tprior = 0.8;
        qinfo[1].sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qinfo[1].pNext              = nullptr;
        qinfo[1].flags              = 0;
        qinfo[1].queueFamilyIndex   = TransferQueueFamily;
        qinfo[1].queueCount         = 1;
        qinfo[1].pQueuePriorities   = &tprior;


        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(PhysicalHandle, &features);

        VkDeviceCreateInfo info;
        info.sType                  = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        info.pNext                  = nullptr;
        info.flags                  = 0;
        info.enabledExtensionCount  = 0;
        info.ppEnabledExtensionNames= nullptr;
        info.enabledLayerCount      = 0;
        info.ppEnabledLayerNames    = nullptr;
        info.pEnabledFeatures       = &features;
        info.queueCreateInfoCount   = TransferQueueFamily != InvalidIndex ? 2 : 1;
        info.pQueueCreateInfos      = qinfo;

        LogInfo("Vulkan: Creating device with % queues", info.queueCreateInfoCount);

        if(vkCreateDevice(PhysicalHandle,&info,nullptr,&Handle) != VK_SUCCESS)
            return Result::Failure;
    }

    {//Obtain Queues
        GraphicsQueue.Obtain(Handle, GraphicsQueueFamily, 0);
        CoreAssert(GraphicsQueue.IsInitialized(), "LogicalDevice: can't retrieve GraphicsQueue handle");
        
        if(TransferQueueFamily != InvalidIndex){
            TransferQueue.Obtain(Handle, TransferQueueFamily, 0);
            CoreAssert(TransferQueue.IsInitialized(), "LogicalDevice: can't retrieve TransferQueue handle");       
        }

        // we need a separate CmdBuffer for transfer operations
        CmdPool.Create(this, TransferQueue.IsInitialized() ? TransferQueue : GraphicsQueue);

        CmdBuffer.Create(&CmdPool);
    }
    return Result::Success;
}

void LogicalGPUImpl::Destroy(){

    CmdBuffer.Destroy();
    CmdPool.Destroy();

    Allocator.Finalize();
    
    vkDestroyDevice(Handle,nullptr);
}

sx_inline bool IsGraphics(VkQueueFlags flags){
    return flags & VK_QUEUE_GRAPHICS_BIT
    &&     flags & VK_QUEUE_COMPUTE_BIT
    &&     flags & VK_QUEUE_TRANSFER_BIT;
}

// pure compute queue // Most probably a DMA engine queue
sx_inline bool IsTransfer(VkQueueFlags flags){
    return !(flags & VK_QUEUE_GRAPHICS_BIT)
    &&     !(flags & VK_QUEUE_COMPUTE_BIT)
    &&       flags & VK_QUEUE_TRANSFER_BIT;
}
// we treat non-graphics queue as compute
sx_inline bool IsCompute(VkQueueFlags flags){
    return  !(flags & VK_QUEUE_GRAPHICS_BIT)
    &&        flags & VK_QUEUE_COMPUTE_BIT
    &&        flags & VK_QUEUE_TRANSFER_BIT;
}

void LogicalGPUImpl::QueryQueues(){
    u32 props_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(PhysicalHandle, &props_count, nullptr);
    VkQueueFamilyProperties *props = (VkQueueFamilyProperties*)alloca(props_count*sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(PhysicalHandle,&props_count,props);

    int i = 0;
    for(; i<props_count; i++){
        if(IsGraphics(props[i].queueFlags))
            GraphicsQueueFamily = i;
        if(IsCompute(props[i].queueFlags))
            ComputeQueueFamily = i;
        if(IsTransfer(props[i].queueFlags))
            TransferQueueFamily = i;
    }

    DLogInfo("Vulkan: GraphicsQueue  %",GraphicsQueueFamily != InvalidIndex);
    DLogInfo("Vulkan: ComputeQueue   %",ComputeQueueFamily != InvalidIndex);
    DLogInfo("Vulkan: TransferQueue  %",TransferQueueFamily != InvalidIndex);


    //higher level queue fallback
    //we are going to have one queue instance of each family
    if(GraphicsQueueFamily != InvalidIndex){
        if(ComputeQueueFamily == InvalidIndex && props[GraphicsQueueFamily].queueCount >= 2){
            ComputeQueueFamily = GraphicsQueueFamily;
            props[GraphicsQueueFamily].queueCount-=1;
            
            DLogInfo("Vulkan: Fallback ComputeQueue   %",ComputeQueueFamily != InvalidIndex);
	    }
        if(TransferQueueFamily == InvalidIndex && props[GraphicsQueueFamily].queueCount >= 2){
            TransferQueueFamily = GraphicsQueueFamily;
            DLogInfo("Vulkan: Fallback TransferQueue  %",TransferQueueFamily != InvalidIndex);
        }
        if(TransferQueueFamily == InvalidIndex && ComputeQueueFamily != InvalidIndex && props[ComputeQueueFamily].queueCount >= 2){
            TransferQueueFamily = ComputeQueueFamily;
            DLogInfo("Vulkan: Fallback TransferQueue  %",TransferQueueFamily != InvalidIndex);
        }
    }

}


}//namespace Vk::
}//namespace StraitX::