#define VK_USE_PLATFORM_MACOS_MVK
#include "graphics/api/vulkan/vulkan.hpp"
#include "core/env/compiler.hpp"
#include <vulkan/vulkan_metal.h>
#include <vulkan/vulkan_macos.h>

namespace Vk{

const char *RequiredPlatformExtensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_EXT_METAL_SURFACE_EXTENSION_NAME,
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};
size_t RequiredPlatformExtensionsCount = sizeof(RequiredPlatformExtensions)/sizeof(char*);

const char *RequiredPlatformLayers[]={
};
size_t RequiredPlatformLayersCount = 0;

}//namespace Vk::