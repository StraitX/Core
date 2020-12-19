#ifndef STRAITX_VULKAN_HPP
#define STRAITX_VULKAN_HPP

#include "platform/platform_detection.hpp"

#ifdef SX_PLATFORM_LINUX
    #include <vulkan/vulkan.h>
#else
    #error "Your platform does not support vulkan yet."
#endif

namespace StraitX{
namespace Vk{

extern const char *RequiredPlatformExtensions[];
extern size_t RequiredPlatformExtensionsCount;
extern const char *RequiredPlatformLayers[];
extern size_t RequiredPlatformLayersCount;

};//namespace Vk::
};//namespace StraitX::


#endif // STRAITX_VULKAN_HPP