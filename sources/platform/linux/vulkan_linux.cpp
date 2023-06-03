#define VK_USE_PLATFORM_XLIB_KHR
#include "graphics/api/vulkan/vulkan.hpp"
#include "core/env/compiler.hpp"

namespace Vk{

const char *RequiredPlatformExtensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};
size_t RequiredPlatformExtensionsCount = sizeof(RequiredPlatformExtensions)/sizeof(char*);

const char *RequiredPlatformLayers[]={
#ifdef SX_DEBUG
    "VK_LAYER_KHRONOS_validation",
#endif
    "VK_LAYER_MESA_overlay"
};
size_t RequiredPlatformLayersCount = sizeof(RequiredPlatformLayers)/sizeof(char*);

}//namespace Vk::