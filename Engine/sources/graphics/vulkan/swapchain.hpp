#ifndef STRAITX_VULKAN_SWAPCHAIN_HPP
#define STRAITX_VULKAN_SWAPCHAIN_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "platform/vk_surface.hpp"
#include "core/math/vector2.hpp"
#include "graphics/vulkan/logical_device.hpp"
#include "graphics/vulkan/fence.hpp"
#include "graphics/vulkan/framebuffer.hpp"
#include "graphics/vulkan/semaphore.hpp"

namespace StraitX{
namespace Vk{

constexpr size_t MaxSwapchainImages = 5;

struct Swapchain{
    Vk::Surface *Surface = nullptr;
    Vk::LogicalDevice *Owner = nullptr;
    VkSwapchainKHR Handle = VK_NULL_HANDLE;

    VkColorSpaceKHR Colorspace;
    VkFormat Format;

    Vector2u SurfaceSize = {0,0}; 

    u32 ImagesCount = 0;
    u32 CurrentImage = -1;
    VkImage Images[MaxSwapchainImages];
    VkImageView ImageViews[MaxSwapchainImages];

    Result Create(Vk::Surface *surface, Vk::LogicalDevice *owner, VkSurfaceFormatKHR format);

    Result CreateChain();

    Result CreateImageViews();

    void AcquireNext(const Vk::Semaphore &signal_semaphore);

    void PresentCurrent(const ArrayPtr<VkSemaphore> &wait_semaphores);

    void DestroyImageViews();

    void DestroyChain();

    void Destroy();
};

};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_SWAPCHAIN_HPP