#include "core/log.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"


namespace StraitX{
namespace Vk{

Result LogicalGPUImpl::Create(const PhysicalGPU &gpu){
    PhysicalHandle = reinterpret_cast<VkPhysicalDevice>(gpu.Handle.U64);
    Vendor = gpu.Vendor;
    Type = gpu.Type;

    QueryMemoryTypes();
    QueryQueues();

    // for now lets keep it simple, we have just one graphics queues
    float prior = 1.0;
    VkDeviceQueueCreateInfo qinfo;
    qinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qinfo.pNext = nullptr;
    qinfo.flags = 0;
    qinfo.queueFamilyIndex = GraphicsQueueFamily;
    qinfo.queueCount = 1;
    qinfo.pQueuePriorities = &prior;

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(PhysicalHandle, &features);



    VkDeviceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.enabledExtensionCount = 0;
    info.ppEnabledExtensionNames = nullptr;
    info.enabledLayerCount = 0;
    info.ppEnabledLayerNames = nullptr;
    info.pEnabledFeatures = &features;
    info.queueCreateInfoCount = 1;
    info.pQueueCreateInfos = &qinfo;

    if(vkCreateDevice(PhysicalHandle,&info,nullptr,&Handle) != VK_SUCCESS)
        return Result::Failure;
    

    
    //                                   we support only one queue of each type
    vkGetDeviceQueue(Handle, GraphicsQueueFamily, 0, &GraphicsQueue.Handle);
    GraphicsQueue.FamilyIndex = GraphicsQueueFamily;
    // here should be a ComputeQueue but it is unused for now

    CoreAssert(GraphicsQueue.Handle != VK_NULL_HANDLE, "LogicalDevice: can't retrieve GraphicsQueue handle");

    return Result::Success;
}

void LogicalGPUImpl::Destroy(){
    vkDestroyDevice(Handle,nullptr);
}


sx_inline bool IsRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    &&      (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT )
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}
sx_inline bool IsUncachedRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT )
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}

sx_inline bool IsVRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    &&     !(type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

sx_inline bool IsDynamicVRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    &&      (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

void LogicalGPUImpl::QueryMemoryTypes(){
    VkPhysicalDeviceMemoryProperties mem_props;
    vkGetPhysicalDeviceMemoryProperties(PhysicalHandle, &mem_props);

    if(mem_props.memoryTypeCount == 1){
        DLogInfo("Vulkan: Exceptional Device: Fallback to one memory type");
        auto &flags = mem_props.memoryTypes[0].propertyFlags;
        if(flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT &&
           flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
        {
            RAM = UncachedRAM = VRAM = DynamicVRAM = 0;
            RAMSize = UncachedRAMSize = VRAMSize = DynamicVRAMSize = mem_props.memoryHeaps[mem_props.memoryTypes[0].heapIndex].size;
            return;
        }
        DLogWarn("Vulkan: Exceptional Device: Unsupported Memory");
        return;
    }
    
    int i = 0; 
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsRAM(mem_props.memoryTypes[i])){
            RAMSize = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            RAM = i++;
            DLogInfo("Vulkan: RAM memory type was found:            Size: % MB",RAMSize/float(1024*1024));
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsUncachedRAM(mem_props.memoryTypes[i])){
            UncachedRAMSize = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            UncachedRAM = i++;
            DLogInfo("Vulkan: UncachedRAM memory type was found:    Size: % MB",UncachedRAMSize/float(1024*1024));
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsVRAM(mem_props.memoryTypes[i])){
            VRAMSize = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            VRAM = i++;
            DLogInfo("Vulkan: VRAM memory type was found:           Size: % MB",VRAMSize/float(1024*1024)); 
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsDynamicVRAM(mem_props.memoryTypes[i])){
            DynamicVRAMSize = mem_props.memoryHeaps[mem_props.memoryTypes[i].heapIndex].size;
            DynamicVRAM = i++;
            DLogInfo("Vulkan: DynamicVRAM memory type was found:    Size: % MB",DynamicVRAMSize/float(1024*1024));
            break;
        }
    }   
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