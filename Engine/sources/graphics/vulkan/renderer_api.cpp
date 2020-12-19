#include <new>
#include <alloca.h>
#include "core/log.hpp"
#include "graphics/vulkan/renderer_api.hpp"

namespace StraitX{
namespace Vk{

RendererAPI RendererAPI::Instance;

constexpr Version VulkanVersion = {1, 0, 0};

static const char *DeviceExtensions[]={
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
constexpr size_t DeviceExtensionsCount = sizeof(DeviceExtensions)/sizeof(char*);

static const char *DeviceLayers[]={};
constexpr size_t DeviceLayersCount = sizeof(DeviceLayers)/sizeof(char*);

Error RendererAPI::InitializeHardware(){
    constexpr Version vk_version = {1,0,0};

    m_ErrInstance = m_Instance.Create(
        VulkanVersion,
        {(char**)RequiredPlatformExtensions, RequiredPlatformExtensionsCount}, 
        {(char**)RequiredPlatformLayers,     RequiredPlatformLayersCount}
    );

    if(m_ErrInstance != Error::Success){
        LogError("Vulkan: Failed to create Instance");
        return m_ErrInstance;
    }

    Error dev = PickBestPhysicalDevice();
    if(dev != Error::Success){
        LogError("Vulkan: Can not find a compatible device");
        return dev;
    }
    

    m_ErrDevice = m_Device.Create(
        &m_PhysicalDevice, 
        {(char**)DeviceExtensions,  DeviceExtensionsCount}, 
        {(char**)DeviceLayers,      DeviceLayersCount}
    );

    if(m_ErrDevice != Error::Success){
	    LogError("Vulkan: Can't create LogicalDevice");
	    return m_ErrDevice;
    }
    return Error::Success;
}

Error RendererAPI::InitializeRender(const Window &window){
    m_ErrSurface = m_Surface.Create(m_Instance.Handle, window);
    if(m_ErrSurface != Error::Success){
	    LogError("Vulkan: Can't create Surface");
        return m_ErrSurface;
    }

    m_ErrSwapchain = m_Swapchain.Create(&m_Surface, &m_Device, m_Format);
    if(m_ErrSwapchain != Error::Success){
	    LogError("Vulkan: Can't create Swapchain");
	    return m_ErrSwapchain;
    }
    return Error::Success;
}

Error RendererAPI::FinalizeRender(){
    if(m_ErrSurface == Error::Success)
        m_Surface.Destroy();
    if(m_ErrSwapchain == Error::Success)
    	m_Swapchain.Destroy();
    return Error::Success;
}

Error RendererAPI::FinalizeHardware(){
    if(m_ErrDevice == Error::Success)
        m_Device.Destroy();

    if(m_ErrInstance == Error::Success)
	m_Instance.Destroy();
    return Error::Success;
}

Error RendererAPI::PickBestPhysicalDevice(){
    ArrayPtr<VkPhysicalDevice, u32> available_devices;
    vkEnumeratePhysicalDevices(m_Instance.Handle, &available_devices.Size,nullptr);
    if(available_devices.Size == 0)
        return Error::NotFound;
    available_devices.Pointer = (VkPhysicalDevice *)alloca(available_devices.Size * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(m_Instance.Handle, &available_devices.Size, available_devices.Pointer);

    ArrayPtr<Vk::PhysicalDevice> devices(nullptr, available_devices.Size);
    devices.Pointer = (Vk::PhysicalDevice*)alloca(devices.Size * sizeof(Vk::PhysicalDevice));

    for(int i = 0; i<devices.Size; i++){
        devices.Pointer[i].Create(available_devices.Pointer[i]);
    }

    int best_index = -1;
    u8 best_score = 0;
    for(int i = 0; i<devices.Size; i++){
        const auto &dev = devices.Pointer[i];
        // it's a game engine, we want to have graphics!
        if(!dev.IsSupported())
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
        return Error::Unsupported;

    m_PhysicalDevice = devices.Pointer[best_index];
    return Error::Success;
}

};//namespace Vk::
};//namespace StraitX::
