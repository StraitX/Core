#include "platform/opengl.hpp"
#include "graphics/opengl/dma_impl.hpp"

namespace StraitX{
namespace GL{

void DMAImpl::CopyCPU2GPUBufferImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 dst_offset){
    glBindBuffer(GL_COPY_WRITE_BUFFER, dst.Handle().U32);
    glBufferSubData(GL_COPY_WRITE_BUFFER, dst_offset, size, src.Pointer());
}

void DMAImpl::CopyCPU2GPUTextureImpl(const CPUTexture &src, const GPUTexture &dst){
    glTextureSubImage2D(dst.Handle().U32, 0, 0, 0, src.Size().x, src.Size().y, GL_RGBA, GL_UNSIGNED_BYTE, src.Pointer());
}

void DMAImpl::ChangeGPUTextureLayoutImpl(GPUTexture &src, GPUTexture::Layout layout){
    src.m_Layout = layout;
}

}//namespace GL::
}//namespace StraitX::