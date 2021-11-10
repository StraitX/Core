#include "core/os/memory.hpp"
#include "core/assert.hpp"
#include "graphics/api/vulkan/queue.hpp"

namespace Vk{

static bool IsGraphics(const VkQueueFamilyProperties &props){
    return (props.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        && (props.queueFlags & VK_QUEUE_TRANSFER_BIT)
        && (props.queueFlags & VK_QUEUE_COMPUTE_BIT);
}

static bool IsCompute(const VkQueueFamilyProperties &props){
    return !(props.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        &&  (props.queueFlags & VK_QUEUE_TRANSFER_BIT)
        &&  (props.queueFlags & VK_QUEUE_COMPUTE_BIT);
}

static bool IsTransfer(const VkQueueFamilyProperties &props){
    return !(props.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        &&  (props.queueFlags & VK_QUEUE_TRANSFER_BIT)
        && !(props.queueFlags & VK_QUEUE_COMPUTE_BIT);
}

QueueProperties QueueProperties::Get(VkPhysicalDevice device){
    QueueProperties result;

    u32 props_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &props_count, nullptr);
    
    auto *props = (VkQueueFamilyProperties*)alloca(sizeof(VkQueueFamilyProperties) * props_count);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &props_count, props);

    for(u32 i = 0; i<props_count; i++){
        if(IsGraphics(props[i])){
            result.Family[QueueFamily::Graphics].Count = props[i].queueCount;
            result.Family[QueueFamily::Graphics].Index = i;
            continue;
        }
        if(IsCompute(props[i])){
            result.Family[QueueFamily::Compute].Count = props[i].queueCount;
            result.Family[QueueFamily::Compute].Index = i;
            continue;
        }
        if(IsTransfer(props[i])){
            result.Family[QueueFamily::Transfer].Count = props[i].queueCount;
            result.Family[QueueFamily::Transfer].Index = i;
            continue;
        }
    }


    
    return result;
}

}//namespace Vk::