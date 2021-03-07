#ifndef STRAITX_GPU_CONTEXT_HPP
#define STRAITX_GPU_CONTEXT_HPP

#include "core/noncopyable.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/gpu_texture.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/api/render_pass.hpp"
#include "graphics/api/framebuffer.hpp"

namespace StraitX{

enum class IndicesType{
    Uint16,
    Uint32
};

class GraphicsAPILoader;

class GPUContext: public NonCopyable{
public:
    struct VTable{
        using NewProc    = GPUContext* (*)(LogicalGPU &owner);
        using DeleteProc = void (*)(GPUContext *);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;

    friend class GraphicsAPILoader;
public:

    GPUContext() = default;

    virtual ~GPUContext() = default;

    virtual void BeginFrame() = 0;

    virtual void EndFrame() = 0;

    virtual void Submit() = 0;

    virtual void Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset = 0) = 0;

    virtual void Bind(const GraphicsPipeline *pipeline) = 0;

    virtual void BeginRenderPass(const RenderPass *pass, const Framebuffer *framebuffer) = 0;

    virtual void EndRenderPass() = 0;

    virtual void BindVertexBuffer(const GPUBuffer &buffer) = 0;

    virtual void BindIndexBuffer(const GPUBuffer &buffer, IndicesType indices_type) = 0;

    virtual void DrawIndexed(u32 indices_count) = 0;

    virtual void ClearFramebufferColorAttachments(const Framebuffer *framebuffer, const Vector4f &color = Vector4f(0, 0, 0, 1.f)) = 0;

    sx_inline static GPUContext *New(){ return s_VTable.New(LogicalGPU::Instance()); }

    sx_inline static void Delete(GPUContext *context){ s_VTable.Delete(context); }
};

}//namespace StraitX::

#endif//STRAITX_GPU_CONTEXT_HPP