#ifndef STRAITX_VULKAN_HPP
#define STRAITX_VULKAN_HPP

#include "platform/platform_detection.hpp"

#if defined(SX_PLATFORM_LINUX) || defined(SX_PLATFORM_WINDOWS)
    #include <vulkan/vulkan.h>
#else
    #error "Your platform does not support vulkan yet."
#endif

#include "platform/memory.hpp"

namespace StraitX{
namespace Vk{

extern const char *RequiredPlatformExtensions[];
extern size_t RequiredPlatformExtensionsCount;
extern const char *RequiredPlatformLayers[];
extern size_t RequiredPlatformLayersCount;

constexpr Version VulkanVersion = {1, 0, 0};

};//namespace Vk::
};//namespace StraitX::


#endif // STRAITX_VULKAN_HPP