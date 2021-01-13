#ifndef STRAITX_VULKAN_DEBUG_HPP
#define STRAITX_VULKAN_DEBUG_HPP

#include "platform/vulkan.hpp"

namespace StraitX{
namespace Vk{

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, 
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
    const VkAllocationCallbacks* pAllocator, 
    VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT messenger, 
    const VkAllocationCallbacks *pAllocator);

}//namespace Vk::
}//namepsace StraitX::

#endif //STRAITX_VULKAN_DEBUG_HPP