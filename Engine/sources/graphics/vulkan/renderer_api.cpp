#include <new>
#include "core/log.hpp"
#include "graphics/vulkan/renderer_api.hpp"
namespace StraitX{
namespace Vk{

RendererAPI RendererAPI::Instance;

Error RendererAPI::InitializeHardware(){
    constexpr Version vk_version = {1,0,0};
    const char *extensions[]={
        
    };
    const char *layers[]={

    };
    if(m_Instance.Create(vk_version, {(char**)RequiredPlatformExtensions, RequiredPlatformExtensionsCount}, 
            {(char**)RequiredPlatformLayers,RequiredPlatformLayersCount}) != Error::Success){
        LogError("Vulkna: Failed to create Instance");
        return Error::Unsupported;
    }

    ArrayPtr<VkPhysicalDevice,u32> devices;
    vkEnumeratePhysicalDevices(m_Instance.Handle,&devices.Size,nullptr);

    if(!devices.Size){
        LogError("Vulkan: Can not find Vulkan Compatible devices");
        return Error::Unsupported;
    }

    m_PhysicalDeivces.Size = devices.Size;
    m_PhysicalDeivces.Pointer = (Vk::PhysicalDevice *)m_Allocator.Alloc(sizeof(Vk::PhysicalDevice)*m_PhysicalDeivces.Size);

    // allocate memory for physical deices to free them in StackAllocator fashion
    devices.Pointer = (VkPhysicalDevice *)m_Allocator.Alloc(devices.Size * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(m_Instance.Handle,&devices.Size,devices.Pointer);

    for(int i = 0; i<devices.Size; i++){
        new (m_PhysicalDeivces.Pointer + i) Vk::PhysicalDevice(devices[i]);
    }
    // here we go
    m_Allocator.Free(devices.Size*sizeof(VkPhysicalDevice));
    
    
    int best_index = -1;
    u8 best_score = 0;
    for(int i = 0; i<m_PhysicalDeivces.Size; i++){
        const auto &dev = m_PhysicalDeivces.Pointer[i];
        // it's a game engine, we want to have graphics!
        if(!dev.IsSupported())
            break;
            
        u8 score =       (dev.Type == DeviceType::DiscreteGPU ? 3 : 0) 
        + (dev.TransferQueueFamily != dev.GraphicsQueueFamily)
        + (dev.ComputeQueueFamily  != dev.TransferQueueFamily);

        if(score > best_score){
            best_index = i;
            best_score = score;
        }
    }

    if(best_index == -1)
        return Error::Unsupported;

    const char *dev_extensions[]={
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    const char *dev_layers[]={

    };
    return m_Device.Create(&m_PhysicalDeivces[best_index], {(char**)dev_extensions, sizeof(dev_extensions)/sizeof(char*)},
        {(char**)dev_layers, sizeof(dev_layers)/sizeof(char*)});
}

Error RendererAPI::InitializeRender(const Window &window){
    if(m_Surface.Create(m_Instance.Handle, window) != Error::Success)
        return Error::Failure;

    return m_Swapchain.Create(m_Format, &m_Device, m_Surface);
}



Error RendererAPI::FinalizeRender(){
    m_Surface.Destroy();
    m_Swapchain.Destroy();
    return Error::Success;
}

Error RendererAPI::FinalizeHardware(){
    m_Device.Destroy();
    return Error::Success;
}


};//namespace Vk::
};//namespace StraitX::