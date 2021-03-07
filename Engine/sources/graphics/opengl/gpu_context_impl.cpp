#include <new>
#include "platform/memory.hpp"
#include "platform/opengl.hpp"
#include "graphics/opengl/debug.hpp"
#include "graphics/opengl/gpu_context_impl.hpp"
#include "graphics/opengl/cpu_buffer_impl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"
#include "graphics/opengl/gpu_texture_impl.hpp"
#include "graphics/opengl/graphics_pipeline_impl.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"

namespace StraitX{
namespace GL{

GLenum GPUContextImpl::s_IndexTypeTable[] = {
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT
};

void GPUContextImpl::Begin(){
    (void)0;
}

void GPUContextImpl::End(){
    m_Pipeline = nullptr;
}

void GPUContextImpl::Submit(){
    glFlush();
}

void GPUContextImpl::Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset){
    CoreAssert(size + dst_offset <= dst.Size(), "GL: GPUContext: Copy: Dst Buffer overflow");

    glBindBuffer(GL_COPY_WRITE_BUFFER, dst.Handle().U32);
    glBufferSubData(GL_COPY_WRITE_BUFFER, dst_offset, size, src.Pointer());
}

void GPUContextImpl::Bind(const GraphicsPipeline *pipeline){
    m_Pipeline = static_cast<const GL::GraphicsPipelineImpl*>(pipeline);
    m_Pipeline->Bind();
}
void GPUContextImpl::BeginRenderPass(const RenderPass *pass, const Framebuffer *framebuffer){
    (void)pass;
    m_Framebuffer = static_cast<const GL::FramebufferImpl *>(framebuffer);
    m_Framebuffer->Bind();
}

void GPUContextImpl::EndRenderPass(){
    m_Framebuffer = nullptr;
}

void GPUContextImpl::BindVertexBuffer(const GPUBuffer &buffer){
    m_Pipeline->BindVertexBuffer(buffer.Handle().U32);
}

void GPUContextImpl::BindIndexBuffer(const GPUBuffer &buffer, IndicesType indices_type){
    m_CurrentIndicesType = s_IndexTypeTable[(size_t)indices_type];
    m_Pipeline->BindIndexBuffer(buffer.Handle().U32);
}

void GPUContextImpl::DrawIndexed(u32 indices_count){
    glDrawElements(m_Pipeline->Topology, indices_count, m_CurrentIndicesType, nullptr);
}

void GPUContextImpl::ClearFramebufferColorAttachments(const Framebuffer *framebuffer, const Vector4f &color){
    CoreAssert(m_Framebuffer != framebuffer, "GL: GPUContextImpl: can't clear framebuffer which is being used in current render pass");
    static_cast<const GL::FramebufferImpl*>(framebuffer)->Bind();
    {
        glClearColor(color.x, color.y, color.z, color.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    if (m_Framebuffer)m_Framebuffer->Bind();
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