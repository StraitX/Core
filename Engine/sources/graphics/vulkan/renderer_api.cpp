#include "platform/memory.hpp"
#include "core/log.hpp"
#include "graphics/vulkan/renderer_api.hpp"

namespace StraitX{
namespace Vk{

RendererAPI RendererAPI::Instance;

static const char *DeviceExtensions[]={
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
constexpr size_t DeviceExtensionsCount = sizeof(DeviceExtensions)/sizeof(char*);

static const char *DeviceLayers[]={
    ""
};
constexpr size_t DeviceLayersCount = sizeof(DeviceLayers)/sizeof(char*);

Result RendererAPI::InitializeHardware(){

    m_ErrInstance = m_Instance.Create(
        VulkanVersion,
        {(const char**)RequiredPlatformExtensions, RequiredPlatformExtensionsCount}, 
        {(const char**)RequiredPlatformLayers,     RequiredPlatformLayersCount}
    );

    if(m_ErrInstance != Result::Success){
        LogError("Vulkan: Failed to create Instance");
        return m_ErrInstance;
    }

    Result dev = PickBestPhysicalDevice();
    if(dev != Result::Success){
        LogError("Vulkan: Can not find a compatible device");
        return dev;
    }
    

    m_ErrDevice = m_Device.Create(
        &m_PhysicalDevice, 
        {(const char**)DeviceExtensions,  DeviceExtensionsCount}, 
        {(const char**)DeviceLayers,      DeviceLayersCount}
    );

    if(m_ErrDevice != Result::Success){
	    LogError("Vulkan: Can't create LogicalDevice");
	    return m_ErrDevice;
    }
    return Result::Success;
}

Result RendererAPI::InitializeRender(const Window &window){
    m_ErrSurface = m_Surface.Create(m_Instance.Handle, window);
    if(m_ErrSurface != Result::Success){
	    LogError("Vulkan: Can't create Surface");
        return m_ErrSurface;
    }

    m_ErrSwapchain = m_Swapchain.Create(&m_Surface, &m_Device, m_Format);
    if(m_ErrSwapchain != Result::Success){
	    LogError("Vulkan: Can't create Swapchain");
	    return m_ErrSwapchain;
    }
    return Result::Success;
}

Result RendererAPI::FinalizeRender(){
    vkQueueWaitIdle(m_Device.GraphicsQueue.Handle);

    if(m_ErrSwapchain == Result::Success)
    	m_Swapchain.Destroy();
    if(m_ErrSurface == Result::Success)
        m_Surface.Destroy();
    return Result::Success;
}

Result RendererAPI::FinalizeHardware(){
    if(m_ErrDevice == Result::Success)
        m_Device.Destroy();
    if(m_ErrInstance == Result::Success)
	    m_Instance.Destroy();
    return Result::Success;
}

Result RendererAPI::PickBestPhysicalDevice(){
    ArrayPtr<VkPhysicalDevice, u32> available_devices;
    vkEnumeratePhysicalDevices(m_Instance.Handle, &available_devices.Size,nullptr);

    if(available_devices.Size == 0)
        return Result::NotFound;
    
    available_devices.Pointer = (VkPhysicalDevice *)alloca(available_devices.Size * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(m_Instance.Handle, &available_devices.Size, available_devices.Pointer);

    ArrayPtr<Vk::PhysicalDevice> devices(nullptr, available_devices.Size);
    devices.Pointer = (Vk::PhysicalDevice*)alloca(devices.Size * sizeof(Vk::PhysicalDevice));

    for(int i = 0; i<devices.Size; i++){
        devices[i].Create(available_devices[i]);
    }

    int best_index = -1;
    u8 best_score = 0;
    for(int i = 0; i<devices.Size; i++){
        const auto &dev = devices[i];
        // it's a game engine, we want to have graphics!
        if(!dev.IsComplete())
            continue;
    	
        u8 score =       (dev.Type == DeviceType::DiscreteGPU ? 3 : 0) 
        + (dev.TransferQueueFamily != dev.GraphicsQueueFamily)
        + (dev.ComputeQueueFamily  != dev.TransferQueueFamily);
	
        if(score > best_score){
            best_index = i;
            best_score = score;
        }
    }
    if(best_index == -1)
        return Result::Unsupported;

    m_PhysicalDevice = devices[best_index];
    return Result::Success;
}

};//namespace Vk::
};//namespace StraitX::
