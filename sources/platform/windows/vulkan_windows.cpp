#define VK_USE_PLATFORM_WIN32_KHR
#include "core/os/vulkan.hpp"
#include "core/env/compiler.hpp"

namespace Vk{

const char *RequiredPlatformExtensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};
size_t RequiredPlatformExtensionsCount = sizeof(RequiredPlatformExtensions)/sizeof(char*);

const char *RequiredPlatformLayers[]={
    nullptr//make msvc happy
};
size_t RequiredPlatformLayersCount = 0;//sizeof(RequiredPlatformLayers)/sizeof(char*);

}//namespace Vk::