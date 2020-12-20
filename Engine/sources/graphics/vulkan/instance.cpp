#include "graphics/vulkan/instance.hpp"

namespace StraitX{
namespace Vk{

Result Instance::Create(const Version &version, const ArrayPtr<char *> &extensions, const ArrayPtr<char *> &layers){
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
    return Result::Success;
}

void Instance::Destroy(){
    vkDestroyInstance(Handle, nullptr);
}

};//namespace Vk::
};//namespace StraitX::