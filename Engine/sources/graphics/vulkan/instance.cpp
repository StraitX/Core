#include <alloca.h>
#include <cstring>
#include "core/log.hpp"
#include "graphics/vulkan/instance.hpp"

namespace StraitX{
namespace Vk{

Result Instance::Create(const Version &version, const ArrayPtr<char *> &extensions, const ArrayPtr<char *> &layers){

    if(CheckExtensions(extensions) == false)
        return Result::Unsupported;
    if(CheckLayers(layers) == false)
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
    
    return ResultError(vkCreateInstance(&create_info, nullptr, &Handle) != VK_SUCCESS);
}

void Instance::Destroy(){
    vkDestroyInstance(Handle, nullptr);
}

bool Instance::CheckLayers(const ArrayPtr<char *> &layers){
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

bool Instance::CheckExtensions(const ArrayPtr<char *> &extensions){
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