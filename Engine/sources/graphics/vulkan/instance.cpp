#include "graphics/vulkan/instance.hpp"

namespace StraitX{
namespace Vk{

Error Instance::Create(const Version &version, const char **extensions, size_t ecount, const char **layers, size_t lcount){
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
    create_info.enabledExtensionCount = ecount;
    create_info.ppEnabledExtensionNames = extensions;
    create_info.enabledLayerCount = lcount;
    create_info.ppEnabledLayerNames = layers;
    
    if(vkCreateInstance(&create_info, nullptr, &Handle) != VK_SUCCESS)
        return Error::Failure;
    return Error::Success;
}

void Instance::Destroy(){
    vkDestroyInstance(Handle, nullptr);
}

};//namespace Vk::
};//namespace StraitX::