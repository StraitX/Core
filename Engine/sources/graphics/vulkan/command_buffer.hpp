#ifndef STRAITX_VULKAN_COMMAND_BUFFER_HPP
#define STRAITX_VULKAN_COMMAND_BUFFER_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "core/array_ptr.hpp"
#include "graphics/vulkan/command_pool.hpp"
#include "graphics/vulkan/fence.hpp"

namespace StraitX{
namespace Vk{

struct RenderPass;
struct Framebuffer;
struct GraphicsPipeline;

struct CommandBuffer{
    VkCommandBuffer Handle = VK_NULL_HANDLE;
    Vk::CommandPool *Pool = nullptr;

    Result Create(Vk::CommandPool *pool);

    void Begin();
                    // here we should bind to graphics pipeline
    void BeginRenderTarget(const Vk::GraphicsPipeline *pipeline, const Vk::RenderPass *render_pass, const Vk::Framebuffer *framebuffer, const ArrayPtr<VkClearValue> &clears);

    void EndRenderTarget();

    void End();

    void Reset();

    void Submit(const ArrayPtr<VkSemaphore> &wait_semaphores, const ArrayPtr<VkSemaphore> &signal_semaphores, const Fence &fence);

    void Destroy();

};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_COMMAND_BUFFER_HPP