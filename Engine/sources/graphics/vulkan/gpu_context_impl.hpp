#ifndef STRAITX_VULKAN_GPU_CONTEXT_IMPL_HPP
#define STRAITX_VULKAN_GPU_CONTEXT_IMPL_HPP

#include "graphics/api/gpu_context.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "vulkan/vulkan_core.h"

namespace StraitX{
namespace Vk{

class GPUContextImpl: public GPUContext{
private:
    static VkIndexType s_IndexTypeTable[];
    Vk::LogicalGPUImpl *m_Owner = nullptr;
    VkCommandPool   m_CmdPool   = VK_NULL_HANDLE;
    VkCommandBuffer m_CmdBuffer = VK_NULL_HANDLE;
public:
    GPUContextImpl(Vk::LogicalGPUImpl *owner);

    ~GPUContextImpl();

    virtual void BeginFrame()override;

    virtual void EndFrame()override;

    virtual void Submit()override;

    virtual void SumbitAsync()override;

    virtual void Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset = 0)override;

    virtual void Bind(const GraphicsPipeline *pipeline)override;

    virtual void BeginRenderPass(const RenderPass *pass, const Framebuffer *framebuffer)override;

    virtual void EndRenderPass()override;

    virtual void BindVertexBuffer(const GPUBuffer &buffer)override;

    virtual void BindIndexBuffer(const GPUBuffer &buffer, IndicesType indices_type)override;

    virtual void DrawIndexed(u32 indices_count)override;

    void CmdPipelineBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst);

    void CmdMemoryBarrier(VkPipelineStageFlags src, VkPipelineStageFlags dst, VkAccessFlags src_acces, VkAccessFlags dst_access);

    static GPUContext *NewImpl(LogicalGPU &owner);

    static void DeleteImpl(GPUContext *context);

};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GPU_CONTEXT_IMPL_HPP