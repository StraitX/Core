#include "platform/opengl.hpp"
#include "graphics/opengl/dma_impl.hpp"
#include "graphics/opengl/texture_impl.hpp"

namespace GL{

void DMAImpl::CopyMem2GPUBufferImpl(const void *src, const GPUBuffer &dst, u32 size, u32 dst_offset){
    glBindBuffer(GL_COPY_WRITE_BUFFER, dst.Handle().U32);
    glBufferSubData(GL_COPY_WRITE_BUFFER, dst_offset, size, src);
}

void DMAImpl::CopyCPU2GPUBufferImpl(const CPUBuffer &src, const GPUBuffer &dst, u32 size, u32 src_offset, u32 dst_offset){
    CopyMem2GPUBufferImpl(((u8*)src.Pointer() + src_offset), dst, size, dst_offset);
}

void DMAImpl::CopyCPU2GPUTextureImpl(const CPUTexture &src, const Texture2D &dst){
    GL::Texture2DImpl::BindZero(dst);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, src.Size().x, src.Size().y, TextureImpl::s_FormatTable[(size_t)src.Format()], TextureImpl::s_TypeTable[(size_t)src.Format()], src.Pointer());
}

void DMAImpl::ChangeGPUTextureLayoutImpl(Texture &src, TextureLayout layout){
    src.m_Layout = layout;
}

}//namespace GL::