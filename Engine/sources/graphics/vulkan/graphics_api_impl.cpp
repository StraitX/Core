#include <cstring>
#include "platform/memory.hpp"
#include "core/log.hpp"
#include "graphics/vulkan/graphics_api_impl.hpp"
#include "graphics/vulkan/debug.hpp"

namespace StraitX{
namespace Vk{

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
    return Create(VulkanVersion, {RequiredPlatformExtensions, RequiredPlatformExtensionsCount}, {RequiredPlatformLayers, RequiredPlatformLayersCount});
}

Result GraphicsAPIImpl::Finalize(){
    Destroy();
    return Result::Success;
}

u32 GraphicsAPIImpl::GetPhysicalGPUCount(){
    u32 count = 0;
    vkEnumeratePhysicalDevices(Handle, &count, nullptr);
    return count;
}

Result GraphicsAPIImpl::GetPhysicalGPUs(PhysicalGPU *array){
    u32 count = GetPhysicalGPUCount();
    auto *devices = (VkPhysicalDevice*)alloca(count * sizeof(VkPhysicalDevice));
    
    if(vkEnumeratePhysicalDevices(Handle, &count, devices) != VK_SUCCESS)
        return Result::Failure;

    for(int i = 0; i<count; i++){
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(devices[i], &props);

        u32 families_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &families_count, nullptr);

        const_cast<GPUHandle&>(array[i].Handle).U64 = reinterpret_cast<u64>(devices[i]);
        const_cast<GPUVendor&>(array[i].Vendor) = VendorIDToVendor(props.vendorID);
        const_cast<GPUType&>(array[i].Type) = VkTypeToGPUType(props.deviceType);
        const_cast<u32&>(array[i].QueueFamiliesCount) = families_count;
    }
    return Result::Success;
}

sx_inline Result GraphicsAPIImpl::Create(const Version &version, const ArrayPtr<const char *> &extensions, const ArrayPtr<const char *> &layers){

    if(!CheckExtensions(extensions))
        return Result::Unsupported;
    if(!CheckLayers(layers))
        return Result::Unsupported;
    
    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.apiVersion = VK_MAKE_VERSION(version.Major, version.Minor, version.Patch);
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pApplicationName = "StraitXClient"; // TODO do something about it
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pEngineName = "StraitX";
    

    VkInstanceCreateInfo create_info;
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pNext = nullptr;
    create_info.flags = 0;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = extensions.Size;
    create_info.ppEnabledExtensionNames = extensions.Pointer;
    create_info.enabledLayerCount = layers.Size;
    create_info.ppEnabledLayerNames = layers.Pointer;
    
    if(vkCreateInstance(&create_info, nullptr, &Handle) != VK_SUCCESS)
        return Result::Failure;

    VkDebugUtilsMessengerCreateInfoEXT debug_info{};
    debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_info.pfnUserCallback = DebugCallback;
    debug_info.pUserData = nullptr; // Optional

    return ResultError(CreateDebugUtilsMessengerEXT(Handle, &debug_info, nullptr, &Messenger) != VK_SUCCESS);
}

sx_inline void GraphicsAPIImpl::Destroy(){
    DestroyDebugUtilsMessengerEXT(Handle, Messenger, nullptr);
    vkDestroyInstance(Handle, nullptr);
}

bool GraphicsAPIImpl::CheckLayers(const ArrayPtr<const char *> &layers){
    u32 count = 0;
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    VkLayerProperties *props = (VkLayerProperties*)alloca(count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&count, props);

    for(auto &lay: layers){
        bool supported = false;
        for(int i = 0; i<count; i++){
            if(strcmp(lay,props[i].layerName) == 0)
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
            if(strcmp(ext,props[i].extensionName) == 0)
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