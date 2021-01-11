#include "graphics/vulkan/logical_device.hpp"


namespace StraitX{
namespace Vk{

Result LogicalDevice::Create(PhysicalDevice *parent, const ArrayPtr<const char*>& extensions, const ArrayPtr<const char*> &layers){
    Parent = parent;
    GraphicsQueue.FamilyIndex = parent->GraphicsQueueFamily;


    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(parent->Handle, &features);

    float priors[2] = {1.0, 0.8};

    // for now lets keep it simple, we have just one graphics queues
    VkDeviceQueueCreateInfo qinfo[1];
    qinfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qinfo[0].pNext = nullptr;
    qinfo[0].flags = 0;
    qinfo[0].queueFamilyIndex = GraphicsQueue.FamilyIndex;
    qinfo[0].queueCount = 1;
    qinfo[0].pQueuePriorities = &priors[0];

    VkDeviceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.enabledExtensionCount = extensions.Size;
    info.ppEnabledExtensionNames = extensions.Pointer;
    info.enabledLayerCount = layers.Size;
    info.ppEnabledLayerNames = layers.Pointer;
    info.pEnabledFeatures = &features;
    info.queueCreateInfoCount = sizeof(qinfo)/sizeof(VkDeviceQueueCreateInfo);
    info.pQueueCreateInfos = qinfo;

    if(vkCreateDevice(Parent->Handle,&info,nullptr,&Handle) != VK_SUCCESS)
        return Result::Failure;
    
    //                                   we support only one queue of each type
    vkGetDeviceQueue(Handle, GraphicsQueue.FamilyIndex, 0, &GraphicsQueue.Handle);
    // here should be a ComputeQueue but it is unused for now

    CoreAssert(GraphicsQueue.Handle != VK_NULL_HANDLE, "LogicalDevice: can't retrieve GraphicsQueue handle");

    return Result::Success;
}

void LogicalDevice::Destroy(){

    vkDestroyDevice(Handle,nullptr);
}

};//namespace Vk::
};//namespace StraitX::