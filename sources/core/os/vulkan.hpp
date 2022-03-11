#ifndef STRAITX_VULKAN_HPP
#define STRAITX_VULKAN_HPP

#include "core/env/os.hpp"

#if defined(SX_OS_LINUX) || defined(SX_OS_WINDOWS) || defined (SX_OS_MACOS)
    #define SX_VULKAN_SUPPORTED
    #include <vulkan/vulkan.h>
#endif

#include "core/types.hpp"

namespace Vk{

extern const char *RequiredPlatformExtensions[];
extern size_t RequiredPlatformExtensionsCount;
extern const char *RequiredPlatformLayers[];
extern size_t RequiredPlatformLayersCount;

constexpr Version VulkanVersion = {1, 1, 0};

};//namespace Vk::

#endif // STRAITX_VULKAN_HPP