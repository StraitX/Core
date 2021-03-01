#ifndef STRAITX_VULKAN_HPP
#define STRAITX_VULKAN_HPP

#include "platform/platform_detection.hpp"

#if defined(SX_PLATFORM_LINUX) || defined(SX_PLATFORM_WINDOWS)
    #define SX_VULKAN_SUPPORTED
    #include <vulkan/vulkan.h>
#endif

#include "platform/types.hpp"

namespace StraitX{
namespace Vk{

extern const char *RequiredPlatformExtensions[];
extern size_t RequiredPlatformExtensionsCount;
extern const char *RequiredPlatformLayers[];
extern size_t RequiredPlatformLayersCount;

constexpr Version VulkanVersion = {1, 1, 0};

};//namespace Vk::
};//namespace StraitX::


#endif // STRAITX_VULKAN_HPP