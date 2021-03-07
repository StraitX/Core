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
    virtual void Begin()override;

    virtual void End()override;

    virtual void Submit()override;

    virtual void Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset = 0)override;

    virtual void Bind(const GraphicsPipeline *pipeline)override;

    virtual void BeginRenderPass(const RenderPass *pass, const Framebuffer *framebuffer)override;

    virtual void EndRenderPass()override;

    virtual void BindVertexBuffer(const GPUBuffer &buffer)override;

    virtual void BindIndexBuffer(const GPUBuffer &buffer, IndicesType indices_type)override;

    virtual void DrawIndexed(u32 indices_count)override;

    virtual void ClearFramebufferColorAttachments(const Framebuffer *framebuffer, const Vector4f &color)override;

    static GPUContext *NewImpl(LogicalGPU &owner);

    static void DeleteImpl(GPUContext *context);

};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_GPU_CONTEXT_IMPL_HPP