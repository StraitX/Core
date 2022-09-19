#include "core/span.hpp"
#include "core/log.hpp"
#include "core/string.hpp"
#include "core/os/memory.hpp"
#include "graphics/api/vulkan/graphics_api_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"

namespace Vk{

bool CheckLayers(const Span<const char *> &layers){
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

bool CheckExtensions(const Span<const char *> &extensions){
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

GraphicsAPIBackendImpl& GraphicsAPIBackendImpl::Get() {
    static GraphicsAPIBackendImpl s_Instance;
    return s_Instance;
}

Result GraphicsAPIBackendImpl::Create(){
    Version version = VulkanVersion;
    Span<const char *>extensions = {RequiredPlatformExtensions, RequiredPlatformExtensionsCount};
    Span<const char *>layers     = {RequiredPlatformLayers, RequiredPlatformLayersCount};


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
    
    if(vkCreateInstance(&info, nullptr, &m_Instance) != VK_SUCCESS)
        return Result::Failure;

    VkDebugUtilsMessengerCreateInfoEXT debug_info{};
    debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_info.pfnUserCallback = DebugCallback;
    debug_info.pUserData = nullptr; // Optional

    if(CreateDebugUtilsMessengerEXT(m_Instance, &debug_info, nullptr, &m_Messenger) != VK_SUCCESS)
        return Result::Failure;
    
    return Result::Success;
}

void GraphicsAPIBackendImpl::Destroy(){
    DestroyDebugUtilsMessengerEXT(m_Instance, m_Messenger, nullptr);
    vkDestroyInstance(m_Instance, nullptr);
}

}//namespace Vk::