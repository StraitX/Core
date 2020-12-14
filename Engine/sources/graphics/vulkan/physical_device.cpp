#include "graphics/vulkan/physical_device.hpp"
#include "core/log.hpp"

namespace StraitX{
namespace Vk{

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

    LogTrace("Vulkan: %, Driver Version %.%.%",
        props.deviceName, 
        VK_VERSION_MAJOR(props.driverVersion),
        VK_VERSION_MAJOR(props.driverVersion),
        VK_VERSION_PATCH(props.driverVersion));

    VkPhysicalDeviceMemoryProperties mem_props;
    vkGetPhysicalDeviceMemoryProperties(Handle, &mem_props);
    
    int i = 0; 
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsRAM(mem_props.memoryTypes[i])){
            RAM = i++;
            DLogInfo("Vulkan: RAM memory type was found");
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsVRAM(mem_props.memoryTypes[i])){
            VRAM = i++;
            DLogInfo("Vulkan: VRAM memory type was found"); 
            break;
        }
    }
    for(i = 0; i < mem_props.memoryTypeCount; i++){
        if(IsDynamicVRAM(mem_props.memoryTypes[i])){
            DynamicVRAM = i++;
            DLogInfo("Vulkan: DynamicVRAM memory type was found");
            break;
        }
    }    

}

};//namespace Vk::
};//namespace StraitX::