#include <alloca.h>
#include <cstring>
#include <cstdio>
#include "core/log.hpp"
#include "graphics/vulkan/instance.hpp"

namespace StraitX{
namespace Vk{

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) 
{
    const char *prefix;
    switch(messageSeverity){
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: prefix = "Trace"; break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: prefix = "Warn "; break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:    prefix = "Info "; break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:   prefix = "Error"; break;
    }

    std::fprintf(stderr,"[%s]: VulkanValidator: \n",prefix);
    std::fprintf(stderr,"%s\n",pCallbackData->pMessage);

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func)
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    else 
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *pAllocator){
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(func)
        func(instance,messenger, pAllocator);
}

Result Instance::Create(const Version &version, const ArrayPtr<const char *> &extensions, const ArrayPtr<const char *> &layers){

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
    debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_info.pfnUserCallback = DebugCallback;
    debug_info.pUserData = nullptr; // Optional

    return ResultError(CreateDebugUtilsMessengerEXT(Handle, &debug_info, nullptr, &Messenger) != VK_SUCCESS);
}

void Instance::Destroy(){
    DestroyDebugUtilsMessengerEXT(Handle, Messenger, nullptr);
    vkDestroyInstance(Handle, nullptr);
}

bool Instance::CheckLayers(const ArrayPtr<const char *> &layers){
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

bool Instance::CheckExtensions(const ArrayPtr<const char *> &extensions){
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