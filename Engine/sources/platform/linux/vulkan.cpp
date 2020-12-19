#define VK_USE_PLATFORM_XLIB_KHR
#include "platform/vulkan.hpp"

namespace StraitX{
namespace Vk{

const char *RequiredPlatformExtensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME
};
size_t RequiredPlatformExtensionsCount = sizeof(RequiredPlatformExtensions)/sizeof(char*);

const char *RequiredPlatformLayers[]={
    
};
size_t RequiredPlatformLayersCount = sizeof(RequiredPlatformLayers)/sizeof(char*);

};//namespace Vk::
};//namespace StraitX::