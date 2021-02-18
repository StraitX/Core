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

void GPUContextImpl::BeginFrame(){
    (void)0;
}

void GPUContextImpl::EndFrame(){
    (void)0;
}

void GPUContextImpl::Submit(){
    glFlush();
}


void GPUContextImpl::SumbitAsync(){
    Submit();
}

void GPUContextImpl::Copy(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset){
    CoreAssert(dst.Size() <= size + dst_offset, "GL: GPUContext: Copy: Dst Buffer overflow");

    glBindBuffer(GL_COPY_WRITE_BUFFER, dst.Handle().U32);
    GL(glBufferSubData(GL_COPY_WRITE_BUFFER, dst_offset, size, src.Pointer()));
}

void GPUContextImpl::Bind(const GraphicsPipeline *pipeline){
    static_cast<const GL::GraphicsPipelineImpl*>(pipeline)->Bind();
}
void GPUContextImpl::BeginRenderPass(const RenderPass *pass, const Framebuffer *framebuffer){
    (void)pass;
    static_cast<const GL::FramebufferImpl *>(framebuffer)->Bind();
}

void GPUContextImpl::EndRenderPass(){
    (void)0;
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