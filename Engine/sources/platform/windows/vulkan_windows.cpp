#define VK_USE_PLATFORM_WIN32_KHR
#include "platform/vulkan.hpp"
#include "platform/compiler.hpp"

namespace StraitX{
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
}//namespace StraitX::