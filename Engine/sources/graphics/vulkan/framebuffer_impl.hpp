#ifndef STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP
#define STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP

#include "graphics/api/framebuffer.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct FramebufferImpl: Framebuffer{
    Vk::LogicalGPUImpl *const Owner;
    VkFramebuffer Handle = VK_NULL_HANDLE;

    FramebufferImpl(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props);

    ~FramebufferImpl();

    static Framebuffer *NewImpl(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props);

    static void DeleteImpl(Framebuffer *framebuffer);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP