#ifndef STRAITX_VULKAN_SURFACE_HPP
#define STRAITX_VULKAN_SURFACE_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "platform/window.hpp"

namespace StraitX{
namespace Vk{

struct Surface{
    VkSurfaceKHR Handle = VK_NULL_HANDLE;
    VkInstance Owner = VK_NULL_HANDLE;

    Result Create(VkInstance owner, const Window &window);

    void Destroy();
};

};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_SURFACE_HPP