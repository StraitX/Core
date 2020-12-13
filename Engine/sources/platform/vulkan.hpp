#ifndef STRAITX_VULKAN_HPP
#define STRAITX_VULKAN_HPP

#include "platform/platform_detection.hpp"

#ifdef SX_PLATFORM_LINUX
    #include <vulkan/vulkan.h>
#else
    #error "Your platform does not support vulkan yet."
#endif

#endif // STRAITX_VULKAN_HPP