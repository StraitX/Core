#include "platform/memory.hpp"
#include "core/log.hpp"
#include "core/string.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/vulkan/graphics_api_impl.hpp"
#include "graphics/vulkan/debug.hpp"
#include "graphics/vulkan/gpu.hpp"

namespace StraitX{
namespace Vk{

GraphicsAPIImpl GraphicsAPIImpl::Instance;

static_assert(sizeof(GPUHandle) == sizeof(VkPhysicalDevice), "GPUHandle is not the same size as VkPhysicalDevice");

GPUType VkTypeToGPUType(u32 type){
    switch (type) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:      return GPUType::Discrete;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:    return GPUType::Integrated;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:       return GPUType::Virtual;
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:             return GPUType::Other;
    default:
        return GPUType::Unknown;
    }
}

Result GraphicsAPIImpl::Initialize(){
    Version version = VulkanVersion;
    ArrayPtr<const char *>extensions = {RequiredPlatformExtensions, RequiredPlatformExtensionsCount};
    ArrayPtr<const char *>layers     = {RequiredPlatformLayers, RequiredPlatformLayersCount};


    if(!CheckExtensions(extensions))
        return Result::Unsupported;
    if(!CheckLayers(layers))
        return Result::Unsupported;
    
    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.apiVersion = VK_MAKE_VERSION(version.Major, version.Minor, version.Patch);
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pApplicationName = "StraitXClient";
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pEngineName = "StraitX";
    

    VkInstanceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.pApplicationInfo = &app_info;
    info.enabledExtensionCount = extensions.Size();
    info.ppEnabledExtensionNames = extensions.Pointer();
    info.enabledLayerCount = layers.Size();
    info.ppEnabledLayerNames = layers.Pointer();
    
    if(vkCreateInstance(&info, nullptr, &m_Handle) != VK_SUCCESS)
        return Result::Failure;

    VkDebugUtilsMessengerCreateInfoEXT debug_info{};
    debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_info.pfnUserCallback = DebugCallback;
    debug_info.pUserData = nullptr; // Optional

    if(CreateDebugUtilsMessengerEXT(m_Handle, &debug_info, nullptr, &m_Messenger) != VK_SUCCESS)
        return Result::Failure;
    //TODO Init GPU and DMA
    return GPU::Get().Initialize(PickBestPhysicalDevice());
}

void GraphicsAPIImpl::Finalize(){
    GPU::Get().Finalize();
    
    DestroyDebugUtilsMessengerEXT(m_Handle, m_Messenger, nullptr);
    vkDestroyInstance(m_Handle, nullptr);
}

int GetPriority(GPUType type){
    switch (type) {
    case GPUType::Unknown: return 0;
    case GPUType::Other: return 0;
    case GPUType::Virtual: return 1;
    case GPUType::Integrated: return 2;
    case GPUType::Discrete: return 3;
    }
}

VkPhysicalDevice GraphicsAPIImpl::PickBestPhysicalDevice()const{
    u32 count = 0;
    vkEnumeratePhysicalDevices(m_Handle, &count, nullptr);

    auto *devices = (VkPhysicalDevice *)alloca(sizeof(VkPhysicalDevice) * count);

    vkEnumeratePhysicalDevices(m_Handle, &count, devices);

    VkPhysicalDevice best = devices[0];
    int best_score = 0;
    
    for(u32 i = 0; i<count; i++){
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(devices[i], &props);

        u32 families_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &families_count, nullptr);

        int score = GetPriority(VkTypeToGPUType(props.deviceType))*10 + families_count * 5; 

        if(score > best_score){
            best = devices[i];
            best_score = score;
        }
    }

    return best;
}

bool GraphicsAPIImpl::CheckLayers(const ArrayPtr<const char *> &layers){
    u32 count = 0;
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    VkLayerProperties *props = (VkLayerProperties*)alloca(count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&count, props);

    for(auto &lay: layers){
        bool supported = false;
        for(int i = 0; i<count; i++){
            if(String::Equals(lay,props[i].layerName))
                supported = true;
        }
        if(!supported){
            LogError("Vulkan: Layer % is not supported",lay);
            return false;
        }
    }

    return true;
}

bool GraphicsAPIImpl::CheckExtensions(const ArrayPtr<const char *> &extensions){
    u32 count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    VkExtensionProperties *props = (VkExtensionProperties *)alloca(count * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(nullptr, &count, props);
    
    for(auto &ext: extensions){
        bool supported = false;
        for(int i = 0; i<count; i++){
            if(String::Equals(ext,props[i].extensionName))
                supported = true;
        }
        if(!supported){
            LogError("Vulkan: Extension % is not supported",ext);
            return false;
        }
    }

    return true; 
}

};//namespace Vk::
};//namespace StraitX::