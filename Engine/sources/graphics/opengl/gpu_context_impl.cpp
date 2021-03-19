#include <new>
#include "graphics/opengl/swapchain_impl.hpp"
#include "platform/memory.hpp"
#include "platform/opengl.hpp"
#include "graphics/opengl/debug.hpp"
#include "graphics/opengl/gpu_context_impl.hpp"
#include "graphics/opengl/cpu_buffer_impl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"
#include "graphics/opengl/gpu_texture_impl.hpp"
#include "graphics/opengl/graphics_pipeline_impl.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"
#include "graphics/opengl/dma_impl.hpp"

namespace StraitX{
namespace GL{

GLenum GPUContextImpl::s_IndexTypeTable[] = {
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT
};

void GPUContextImpl::BeginImpl(){
    (void)0;
}

void GPUContextImpl::EndImpl(){
    m_Pipeline = nullptr;
}

void GPUContextImpl::SubmitImpl(){
    glFlush();
}

void GPUContextImpl::CopyImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset){
    DMAImpl::CopyCPU2GPUBufferImpl(src, dst, size, src_offset, dst_offset);
}

void GPUContextImpl::CopyImpl(const CPUTexture &src, const GPUTexture &dst){
    DMAImpl::CopyCPU2GPUTextureImpl(src, dst);
}

void GPUContextImpl::ChangeLayoutImpl(GPUTexture &texture, GPUTexture::Layout new_layout){
    texture.m_Layout = new_layout;
}

void GPUContextImpl::BindImpl(const GraphicsPipeline *pipeline){
    m_Pipeline = static_cast<const GL::GraphicsPipelineImpl*>(pipeline);
    m_Pipeline->Bind();
}
void GPUContextImpl::BeginRenderPassImpl(const RenderPass *pass, const Framebuffer *framebuffer){
    (void)pass;
    m_Framebuffer = static_cast<const GL::FramebufferImpl *>(framebuffer);
    m_Framebuffer->Bind();
}

void GPUContextImpl::EndRenderPassImpl(){
    m_Framebuffer = nullptr;
}

void GPUContextImpl::BindVertexBufferImpl(const GPUBuffer &buffer){
    m_Pipeline->BindVertexBuffer(buffer.Handle().U32);
}

void GPUContextImpl::BindIndexBufferImpl(const GPUBuffer &buffer, IndicesType indices_type){
    m_CurrentIndicesType = s_IndexTypeTable[(size_t)indices_type];
    m_Pipeline->BindIndexBuffer(buffer.Handle().U32);
}

void GPUContextImpl::DrawIndexedImpl(u32 indices_count){
    glDrawElements(m_Pipeline->Topology, indices_count, m_CurrentIndicesType, nullptr);
}

void GPUContextImpl::ClearFramebufferColorAttachmentsImpl(const Framebuffer *framebuffer, const Vector4f &color){
    CoreAssert(m_Framebuffer != framebuffer, "GL: GPUContextImpl: can't clear framebuffer which is being used in current render pass");
    static_cast<const GL::FramebufferImpl*>(framebuffer)->Bind();
    {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    if (m_Framebuffer)m_Framebuffer->Bind();
}

void GPUContextImpl::SwapFramebuffersImpl(Swapchain *swapchain){
    static_cast<GL::SwapchainImpl*>(swapchain)->m_Owner->SwapBuffers();
}

GPUContext *GPUContextImpl::NewImpl(LogicalGPU &owner){
    // In OpenGL we don't have GPU distinctions so owner doesn't matter
    (void)owner;
    return new(Memory::Alloc(sizeof(GPUContextImpl))) GPUContextImpl();
}

void GPUContextImpl::DeleteImpl(GPUContext *context){
    context->~GPUContext();
    Memory::Free(context);
}

}//namespace GL::
}//namespace StraitX::