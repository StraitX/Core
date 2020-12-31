#ifndef STRAITX_VULKAN_COMMAND_BUFFER_HPP
#define STRAITX_VULKAN_COMMAND_BUFFER_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "graphics/vulkan/command_pool.hpp"
#include "graphics/vulkan/render_pass.hpp"
#include "graphics/vulkan/framebuffer.hpp"
#include "graphics/vulkan/graphics_pipeline.hpp"

namespace StraitX{
namespace Vk{

struct CommandBuffer{
    VkCommandBuffer Handle = VK_NULL_HANDLE;
    Vk::CommandPool *Pool = nullptr;

    Result Create(Vk::CommandPool *pool);

    void Begin();
                    // here we should bind to graphics pipeline
    void BeginRenderTarget(Vk::GraphicsPipeline *pipeline, Vk::RenderPass *render_pass, Vk::Framebuffer *framebuffer, const ArrayPtr<VkClearValue> &clears);

    void EndRenderTarget();

    void End();

    void Reset();

    void Submit(const ArrayPtr<VkSemaphore> &wait_semaphores, const ArrayPtr<VkSemaphore> &signal_semaphores);

    void Destroy();

};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_COMMAND_BUFFER_HPP