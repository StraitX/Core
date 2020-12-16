#include <alloca.h>
#include "core/log.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/physical_device.hpp"

namespace StraitX{
namespace Vk{


const char *DeviceTypes[]={
    "Unknown",
    "DiscreteGPU",
    "IntegratedGPU"
};

PhysicalDevice::PhysicalDevice(VkPhysicalDevice device):
    Handle(device)
{
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(Handle,&props);
    switch (props.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: Type = DeviceType::DiscreteGPU; break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: Type = DeviceType::IntegratedGPU; break;
    default:
        Type = DeviceType::Unknown;
        DLogWarn("Vulkan: Unknown DeviceType");
    }

    LogTrace("Vulkan: [%] %, Driver Version %.%.%",
        DeviceTypes[(int)Type],
        props.deviceName, 
        VK_VERSION_MAJOR(props.driverVersion),
        VK_VERSION_MAJOR(props.driverVersion),
        VK_VERSION_PATCH(props.driverVersion)); 

    QueryMemoryTypes();
    QueryQueues();
}


sx_inline bool IsRAM(VkMemoryType type){
    return  (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
    &&      (type.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT )
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

void PhysicalDevice::QueryMemoryTypes(){
    VkPhysicalDeviceMemoryProperties mem_props;
    vkGetPhysicalDeviceMemoryProperties(Handle, &mem_props);
    
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

void PhysicalDevice::QueryQueues(){
    u32 props_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(Handle, &props_count, nullptr);
    VkQueueFamilyProperties *props = (VkQueueFamilyProperties*)alloca(props_count*sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(Handle,&props_count,props);

    int i = 0;
    for(; i<props_count; i++){
        if(IsGraphics(props[i].queueFlags))
            GraphicsQueueFamily = i;
        if(IsCompute(props[i].queueFlags))
            ComputeQueueFamily = i;
        if(IsTransfer(props[i].queueFlags))
            TransferQueueFamily = i;
    }

    DLogInfo("Vulkan: GraphicsQueue  %",GraphicsQueueFamily != -1);
    DLogInfo("Vulkan: ComputeQueue   %",ComputeQueueFamily != -1);
    DLogInfo("Vulkan: TransferQueue  %",TransferQueueFamily != -1);


    //higher level queue fallback
    //we are going to have one queue instance of each family
    if(GraphicsQueueFamily != -1){
        if(ComputeQueueFamily == -1 && props[GraphicsQueueFamily].queueCount >= 2){
            ComputeQueueFamily = GraphicsQueueFamily;
            props[GraphicsQueueFamily].queueCount-=1;
        }
        if(TransferQueueFamily == -1 && props[ComputeQueueFamily].queueCount >= 2){
            TransferQueueFamily = ComputeQueueFamily;
        }
    }

}

};//namespace Vk::
};//namespace StraitX::