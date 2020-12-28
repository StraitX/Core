#ifndef STRAITX_VULKAN_FRAMEBUFFER_HPP
#define STRAITX_VULKAN_FRAMEBUFFER_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "platform/types.hpp"
#include "core/array_ptr.hpp"
#include "core/math/vector2.hpp"
#include "graphics/vulkan/logical_device.hpp"
#include "graphics/vulkan/render_pass.hpp"

namespace StraitX{
namespace Vk{

struct Framebuffer{
    VkFramebuffer Handle = VK_NULL_HANDLE;
    Vk::RenderPass *RenderPass = nullptr;
    Vector2u Size = {0, 0};

    Result Create(Vk::RenderPass *render_pass, const Vector2u &size, const ArrayPtr<VkImageView> &attachments);

    void Destroy();
};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_FRAMEBUFFER_HPP