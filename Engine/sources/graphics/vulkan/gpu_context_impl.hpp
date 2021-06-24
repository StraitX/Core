#ifndef STRAITX_VULKAN_GPU_CONTEXT_IMPL_HPP
#define STRAITX_VULKAN_GPU_CONTEXT_IMPL_HPP

#include "core/pair.hpp"
#include "graphics/api/gpu_context.hpp"
#include "graphics/vulkan/command_buffer.hpp"
#include "graphics/vulkan/semaphore.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/framebuffer_impl.hpp"
#include "graphics/vulkan/graphics_pipeline_impl.hpp"

namespace StraitX{
namespace Vk{

class GPUContextImpl: public GPUContext{
private:
    static VkIndexType s_IndexTypeTable[];
    Vk::CommandBuffer m_CmdBuffer{QueueFamily::Graphics};

    static constexpr size_t SemaphoreRingSize = 2;
    Vk::Semaphore m_SemaphoreRing[SemaphoreRingSize];
    VkCommandBuffer m_CmdBufferRing[SemaphoreRingSize];
    u8 m_SemaphoreRingCounter = 0;

	const Vk::GraphicsPipelineImpl *m_Pipeline = nullptr;

    const Vk::RenderPassImpl *m_RenderPass = nullptr;
    const Vk::FramebufferImpl *m_Framebuffer = nullptr;
public:
    GPUContextImpl();

    virtual void BeginImpl()override;

    virtual void EndImpl()override;

    virtual void SubmitImpl()override;

    virtual void CopyImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset)override;

    virtual void CopyImpl(const CPUTexture &src, const GPUTexture &dst)override;

    virtual void ChangeLayoutImpl(GPUTexture &texture, GPUTexture::Layout new_layout)override;

    virtual void BindImpl(const GraphicsPipeline *pipeline)override;

	virtual void BindDescriptorSetImpl(const DescriptorSet *set)override;

    virtual void BeginRenderPassImpl(const RenderPass *pass, const Framebuffer *framebuffer)override;

    virtual void EndRenderPassImpl()override;

    virtual void BindVertexBufferImpl(const GPUBuffer &buffer)override;

    virtual void BindIndexBufferImpl(const GPUBuffer &buffer, IndicesType indices_type)override;

    virtual void DrawIndexedImpl(u32 indices_count)override;

    virtual void ClearFramebufferColorAttachmentsImpl(const Framebuffer *fb, const Vector4f &color)override;

    virtual void ClearFramebufferDepthAttachmentsImpl(const Framebuffer *fb, float value)override;

    virtual void SwapFramebuffersImpl(Swapchain *swapchain)override;

    Pair<VkSemaphore, VkSemaphore> NextPair();

    static GPUContext *NewImpl();

    static void DeleteImpl(GPUContext *context);

};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GPU_CONTEXT_IMPL_HPP