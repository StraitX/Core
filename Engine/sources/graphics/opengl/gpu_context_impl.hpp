#ifndef STRAITX_OPENGL_GPU_CONTEXT_IMPL_HPP
#define STRAITX_OPENGL_GPU_CONTEXT_IMPL_HPP

#include "graphics/api/gpu_context.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"
#include "graphics/opengl/graphics_pipeline_impl.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"

namespace StraitX{
namespace GL{

class GPUContextImpl: public GPUContext{
private:
    static GLenum s_IndexTypeTable[];
    const GL::GraphicsPipelineImpl *m_Pipeline = nullptr;
    const GL::FramebufferImpl *m_Framebuffer = nullptr;
    GLenum m_CurrentIndicesType;
public:
    virtual void BeginImpl()override;

    virtual void EndImpl()override;

    virtual void SubmitImpl()override;

    virtual void CopyImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset = 0)override;

    virtual void BindImpl(const GraphicsPipeline *pipeline)override;

    virtual void BeginRenderPassImpl(const RenderPass *pass, const Framebuffer *framebuffer)override;

    virtual void EndRenderPassImpl()override;

    virtual void BindVertexBufferImpl(const GPUBuffer &buffer)override;

    virtual void BindIndexBufferImpl(const GPUBuffer &buffer, IndicesType indices_type)override;

    virtual void DrawIndexedImpl(u32 indices_count)override;

    virtual void ClearFramebufferColorAttachmentsImpl(const Framebuffer *framebuffer, const Vector4f &color)override;

    virtual void SwapFramebuffersImpl(Swapchain *swapchain)override;

    static GPUContext *NewImpl(LogicalGPU &owner);

    static void DeleteImpl(GPUContext *context);

};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_GPU_CONTEXT_IMPL_HPP