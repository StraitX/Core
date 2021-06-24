#ifndef STRAITX_GPU_CONTEXT_HPP
#define STRAITX_GPU_CONTEXT_HPP

#include "platform/compiler.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/cpu_texture.hpp"
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

class GPUContext: public NonCopyable{
public:
    struct VTable{
        using NewProc    = GPUContext* (*)();
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
    static GPUContext *s_Instance;

    friend class GraphicsAPILoader;
    // XXX just for now
    friend class RenderWindow;
public:

    GPUContext() = default;

    virtual ~GPUContext() = default;
    
    void Begin();

    void End();

    void Submit();

    void Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset = 0, u32 dst_offset = 0);

    void Copy(const CPUBuffer &src, const GPUBuffer &dst);

    void Copy(const CPUTexture &src, const GPUTexture &dst);

    void ChangeLayout(GPUTexture &texture, GPUTexture::Layout new_layout);

    void Bind(const GraphicsPipeline *pipeline);

	void BindDescriptorSet(const DescriptorSet *set);

    void BeginRenderPass(const RenderPass *pass, const Framebuffer *framebuffer);

    void EndRenderPass();

    void BindVertexBuffer(const GPUBuffer &buffer);

    void BindIndexBuffer(const GPUBuffer &buffer, IndicesType indices_type);

    void DrawIndexed(u32 indices_count);

    void ClearFramebufferColorAttachments(const Framebuffer *framebuffer, const Vector4f &color = Vector4f(0, 0, 0, 1.f));

    void ClearFramebufferDepthAttachments(const Framebuffer *framebuffer, float value = 1.f);

    void SwapFramebuffers(Swapchain *swapchain);

protected:
    virtual void BeginImpl() = 0;

    virtual void EndImpl() = 0;

    virtual void SubmitImpl() = 0;

    virtual void CopyImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset) = 0;

    virtual void CopyImpl(const CPUTexture &src, const GPUTexture &dst) = 0;

    virtual void ChangeLayoutImpl(GPUTexture &texture, GPUTexture::Layout new_layout) = 0;

    virtual void BindImpl(const GraphicsPipeline *pipeline) = 0;
	
	virtual void BindDescriptorSetImpl(const DescriptorSet *set) = 0;

    virtual void BeginRenderPassImpl(const RenderPass *pass, const Framebuffer *framebuffer) = 0;

    virtual void EndRenderPassImpl() = 0;

    virtual void BindVertexBufferImpl(const GPUBuffer &buffer) = 0;

    virtual void BindIndexBufferImpl(const GPUBuffer &buffer, IndicesType indices_type) = 0;

    virtual void DrawIndexedImpl(u32 indices_count) = 0;

    virtual void ClearFramebufferColorAttachmentsImpl(const Framebuffer *framebuffer, const Vector4f &color) = 0;

    virtual void ClearFramebufferDepthAttachmentsImpl(const Framebuffer *framebuffer, float value) = 0;

    virtual void SwapFramebuffersImpl(Swapchain *swapchain) = 0;
public:
    static GPUContext *Get(){ return s_Instance; }
private:
    static GPUContext *New(){ return s_VTable.New(); }

    static void Delete(GPUContext *context){ s_VTable.Delete(context); }
};



SX_INLINE void GPUContext::Begin(){
#ifdef SX_DEBUG
    SX_ASSERT(m_State == State::Recordable);
    m_PipelineIsBound = false;
    m_VertexBufferIsBound = false;
    m_IndexBufferIsBound = false;
    m_State = State::Recording;
#endif
    BeginImpl();
}

SX_INLINE void GPUContext::End(){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(m_State != State::InPass,"You should EndRenderPass() before End() of GPUContext");
    SX_ASSERT(m_State == State::Recording);
    m_State = State::Submitable;
#endif
    EndImpl(); 
}

SX_INLINE void GPUContext::Submit(){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(m_State == State::Submitable, "Context should be in submitable state before Submit()");
    m_State = State::Recordable;
#endif
    SubmitImpl();
}

SX_INLINE void GPUContext::Copy(const CPUBuffer &src, const GPUBuffer &dst){
    SX_CORE_ASSERT(src.Size() == dst.Size(), "GPUContext::Copy, buffers should be the same size");
    Copy(src, dst, dst.Size());
}

SX_INLINE void GPUContext::Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(m_State != State::InPass,"You can't Copy() inside render pass");

    SX_CORE_ASSERT(m_State == State::Recording,"Context should be in recording state before Copy()");
    SX_CORE_ASSERT(dst.Usage() & GPUBuffer::TransferDestination,"copy destination buffer should be created with GPUBuffer::TransferDestination usage flag");
#endif
    CopyImpl(src, dst, size, src_offset, dst_offset);
}

SX_INLINE void GPUContext::Copy(const CPUTexture &src, const GPUTexture &dst){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(src.Size() == dst.Size(), "Source texture size and destination size should match");
    SX_CORE_ASSERT(src.Format() == dst.GetFormat(),"src and dst should have same format");
    SX_CORE_ASSERT(dst.GetUsage() & GPUTexture::TransferDst, "Texture should be created with TransferDst usage flag");
    SX_CORE_ASSERT(dst.GetLayout() == GPUTexture::Layout::TransferDstOptimal || dst.GetLayout() == GPUTexture::Layout::General, "Destination texture should be in TransferDstOptimal or General layout");
#endif
    CopyImpl(src,dst);
}

SX_INLINE void GPUContext::ChangeLayout(GPUTexture &texture, GPUTexture::Layout new_layout){
    SX_CORE_ASSERT(texture.GetLayout() != new_layout, "Texture is already in new_layout");
    ChangeLayoutImpl(texture, new_layout);
}

SX_INLINE void GPUContext::Bind(const GraphicsPipeline *pipeline){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(pipeline, "GraphicsPipeline* should not be nullptr");
    m_PipelineIsBound = true;
#endif
    BindImpl(pipeline);
}

SX_INLINE void GPUContext::BindDescriptorSet(const DescriptorSet *set){
#ifdef SX_DEBUG
	SX_CORE_ASSERT(m_PipelineIsBound, "GraphicsPipeline should be bound in order to bind descriptor set");
#endif
	BindDescriptorSetImpl(set);
}

SX_INLINE void GPUContext::BeginRenderPass(const RenderPass *pass, const Framebuffer *framebuffer){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(pass, "RenderPass* should not be nullptr");
    SX_CORE_ASSERT(framebuffer, "Framebuffer* should not be nullptr");

    SX_CORE_ASSERT(m_State == State::Recording, "Context should be in recording state before BeginRenderPass()");
    m_State = State::InPass;
#endif
    BeginRenderPassImpl(pass, framebuffer);
}

SX_INLINE void GPUContext::EndRenderPass(){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(m_State == State::InPass, "Context should be in InPass state before EndRenderPass()");
    m_State = State::Recording;
#endif
    EndRenderPassImpl(); 
}

SX_INLINE void GPUContext::BindVertexBuffer(const GPUBuffer &buffer){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(m_PipelineIsBound, "You can't bind vertex buffer without GraphicsPipeline being binded");
    SX_CORE_ASSERT(buffer.Usage() & GPUBuffer::VertexBuffer, "you can bind as vertex buffer only GPUBuffers created with VertexBuffer usage flag");
    m_VertexBufferIsBound = true;
#endif
    BindVertexBufferImpl(buffer); 
}

SX_INLINE void GPUContext::BindIndexBuffer(const GPUBuffer &buffer, IndicesType indices_type){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(m_PipelineIsBound, "You can't bind index buffer without GraphicsPipeline being binded");
    SX_CORE_ASSERT(buffer.Usage() & GPUBuffer::IndexBuffer, "you can bind as index buffer only GPUBuffers created with IndexBuffer usage flag");
    m_IndexBufferIsBound = true;
#endif
    BindIndexBufferImpl(buffer, indices_type);
}

SX_INLINE void GPUContext::DrawIndexed(u32 indices_count){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(m_State == State::InPass, "you can't issue draw call outside of the render pass");
    SX_CORE_ASSERT(indices_count, "can't draw zero indices");

    SX_CORE_ASSERT(m_VertexBufferIsBound,"you can't DrawIndexed without VertexBuffer being binded");
    SX_CORE_ASSERT(m_IndexBufferIsBound,"you can't DrawIndexed without IndexBuffer being binded");
#endif
    DrawIndexedImpl(indices_count); 
}

SX_INLINE void GPUContext::ClearFramebufferColorAttachments(const Framebuffer *framebuffer, const Vector4f &color){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(m_State != State::InPass, "you can't clear framebuffer attachments inside of the render pass");
    SX_CORE_ASSERT(framebuffer, "Framebuffer* should not be nullptr");
#endif
    ClearFramebufferColorAttachmentsImpl(framebuffer, color);
}

SX_INLINE void GPUContext::ClearFramebufferDepthAttachments(const Framebuffer *framebuffer, float value){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(m_State != State::InPass, "you can't clear framebuffer attachments inside of the render pass");
    SX_CORE_ASSERT(framebuffer, "Framebuffer* should not be nullptr");
#endif
    ClearFramebufferDepthAttachmentsImpl(framebuffer, value);
}

SX_INLINE void GPUContext::SwapFramebuffers(Swapchain *swapchain){
#ifdef SX_DEBUG
    SX_CORE_ASSERT(m_State == State::Recordable, "You can't SwapBuffers during recording");
    SX_CORE_ASSERT(swapchain, "Swapchain* should not be nullptr");
#endif
    SwapFramebuffersImpl(swapchain); 
}

}//namespace StraitX::

#endif//STRAITX_GPU_CONTEXT_HPP