#ifndef STRAITX_VULKAN_DEBUG_HPP
#define STRAITX_VULKAN_DEBUG_HPP

#include "core/os/vulkan.hpp"
#include "core/assert.hpp"

#define SX_VK_ASSERT(func, msg) SX_CORE_CALL_ASSERT(func, VK_SUCCESS, msg)

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

#endif //STRAITX_VULKAN_DEBUG_HPP