#ifndef STRAITX_GPU_CONTEXT_HPP
#define STRAITX_GPU_CONTEXT_HPP

#include "platform/compiler.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/gpu_texture.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/api/swapchain.hpp"
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
//Track state of GPUContext to prevent api incompatability bugs
#ifdef SX_DEBUG
    enum class State{
        Recordable,
        Recording,
        InPass,
        Submitable
    };
    State m_State = State::Recordable;
    bool m_PipelineIsBound = false;
    bool m_IndexBufferIsBound = false;
    bool m_VertexBufferIsBound = false;
#endif
    static VTable s_VTable;

    friend class GraphicsAPILoader;
public:

    GPUContext() = default;

    virtual ~GPUContext() = default;
    
    void Begin();

    void End();

    void Submit();

    void Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset = 0);

    void Bind(const GraphicsPipeline *pipeline);

    void BeginRenderPass(const RenderPass *pass, const Framebuffer *framebuffer);

    void EndRenderPass();

    void BindVertexBuffer(const GPUBuffer &buffer);

    void BindIndexBuffer(const GPUBuffer &buffer, IndicesType indices_type);

    void DrawIndexed(u32 indices_count);

    void ClearFramebufferColorAttachments(const Framebuffer *framebuffer, const Vector4f &color = Vector4f(0, 0, 0, 1.f));

    void SwapFramebuffers(Swapchain *swapchain);

protected:
    virtual void BeginImpl() = 0;

    virtual void EndImpl() = 0;

    virtual void SubmitImpl() = 0;

    virtual void CopyImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset) = 0;

    virtual void BindImpl(const GraphicsPipeline *pipeline) = 0;

    virtual void BeginRenderPassImpl(const RenderPass *pass, const Framebuffer *framebuffer) = 0;

    virtual void EndRenderPassImpl() = 0;

    virtual void BindVertexBufferImpl(const GPUBuffer &buffer) = 0;

    virtual void BindIndexBufferImpl(const GPUBuffer &buffer, IndicesType indices_type) = 0;

    virtual void DrawIndexedImpl(u32 indices_count) = 0;

    virtual void ClearFramebufferColorAttachmentsImpl(const Framebuffer *framebuffer, const Vector4f &color) = 0;

    virtual void SwapFramebuffersImpl(Swapchain *swapchain) = 0;
public:
    sx_inline static GPUContext *New(){ return s_VTable.New(LogicalGPU::Instance()); }

    sx_inline static void Delete(GPUContext *context){ s_VTable.Delete(context); }
};



sx_inline void GPUContext::Begin(){
#ifdef SX_DEBUG
    Assert(m_State == State::Recordable);
    m_PipelineIsBound = false;
    m_VertexBufferIsBound = false;
    m_IndexBufferIsBound = false;
    m_State = State::Recording;
#endif
    BeginImpl();
}

sx_inline void GPUContext::End(){
#ifdef SX_DEBUG
    CoreAssert(m_State != State::InPass,"You should EndRenderPass() before End() of GPUContext");
    Assert(m_State == State::Recording);
    m_State = State::Submitable;
#endif
    EndImpl(); 
}

sx_inline void GPUContext::Submit(){
#ifdef SX_DEBUG
    CoreAssert(m_State == State::Submitable, "Context should be in submitable state before Submit()");
    m_State = State::Recordable;
#endif
    SubmitImpl();
}

sx_inline void GPUContext::Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset){
#ifdef SX_DEBUG
    CoreAssert(m_State != State::InPass,"You can't Copy() inside render pass");

    CoreAssert(m_State == State::Recording,"Context should be in recording state before Copy()");
    CoreAssert(dst.Usage() & GPUBuffer::TransferDestination,"copy destination buffer should be created with GPUBuffer::TransferDestination usage flag");
#endif
    CopyImpl(src, dst, size, dst_offset);
}

sx_inline void GPUContext::Bind(const GraphicsPipeline *pipeline){
#ifdef SX_DEBUG
    CoreAssert(pipeline, "GraphicsPipeline* should not be nullptr");
    m_PipelineIsBound = true;
#endif
    BindImpl(pipeline);
}

sx_inline void GPUContext::BeginRenderPass(const RenderPass *pass, const Framebuffer *framebuffer){
#ifdef SX_DEBUG
    CoreAssert(pass, "RenderPass* should not be nullptr");
    CoreAssert(framebuffer, "Framebuffer* should not be nullptr");

    CoreAssert(m_State == State::Recording, "Context should be in recording state before BeginRenderPass()");
    m_State = State::InPass;
#endif
    BeginRenderPassImpl(pass, framebuffer);
}

sx_inline void GPUContext::EndRenderPass(){
#ifdef SX_DEBUG
    CoreAssert(m_State == State::InPass, "Context should be in InPass state before EndRenderPass()");
    m_State = State::Recording;
#endif
    EndRenderPassImpl(); 
}

sx_inline void GPUContext::BindVertexBuffer(const GPUBuffer &buffer){
#ifdef SX_DEBUG
    CoreAssert(m_PipelineIsBound, "You can't bind vertex buffer without GraphicsPipeline being binded");
    CoreAssert(buffer.Usage() & GPUBuffer::VertexBuffer, "you can bind as vertex buffer only GPUBuffers created with VertexBuffer usage flag");
    m_VertexBufferIsBound = true;
#endif
    BindVertexBufferImpl(buffer); 
}

sx_inline void GPUContext::BindIndexBuffer(const GPUBuffer &buffer, IndicesType indices_type){
#ifdef SX_DEBUG
    CoreAssert(m_PipelineIsBound, "You can't bind index buffer without GraphicsPipeline being binded");
    CoreAssert(buffer.Usage() & GPUBuffer::IndexBuffer, "you can bind as index buffer only GPUBuffers created with IndexBuffer usage flag");
    m_IndexBufferIsBound = true;
#endif
    BindIndexBufferImpl(buffer, indices_type);
}

sx_inline void GPUContext::DrawIndexed(u32 indices_count){
#ifdef SX_DEBUG
    CoreAssert(m_State == State::InPass, "you can't issue draw call outside of the render pass");
    CoreAssert(indices_count, "can't draw zero indices");

    CoreAssert(m_VertexBufferIsBound,"you can't DrawIndexed without VertexBuffer being binded");
    CoreAssert(m_IndexBufferIsBound,"you can't DrawIndexed without IndexBuffer being binded");
#endif
    DrawIndexedImpl(indices_count); 
}

sx_inline void GPUContext::ClearFramebufferColorAttachments(const Framebuffer *framebuffer, const Vector4f &color){
#ifdef SX_DEBUG
    CoreAssert(m_State != State::InPass, "you can't issue draw call inside of the render pass");
    CoreAssert(framebuffer, "Framebuffer* should not be nullptr");
#endif
    ClearFramebufferColorAttachmentsImpl(framebuffer, color);
}

sx_inline void GPUContext::SwapFramebuffers(Swapchain *swapchain){
#ifdef SX_DEBUG
    CoreAssert(m_State == State::Recordable, "You can't SwapBuffers during recording");
    CoreAssert(swapchain, "Swapchain* should not be nullptr");
#endif
    SwapFramebuffersImpl(swapchain); 
}

}//namespace StraitX::

#endif//STRAITX_GPU_CONTEXT_HPP