#include "core/os/opengl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"

namespace GL{

void GPUBufferImpl::NewImpl(GPUBuffer &buffer, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage){
    buffer.m_Size = size;
    buffer.m_Usage = usage;
    buffer.m_MemoryType = mem_type;
    
    glGenBuffers(1, &buffer.m_Handle.U32);
    glBindBuffer(GL_COPY_WRITE_BUFFER, buffer.m_Handle.U32);

    glBufferData(GL_COPY_WRITE_BUFFER, buffer.m_Size, nullptr, mem_type == GPUMemoryType::VRAM ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}

void GPUBufferImpl::DeleteImpl(GPUBuffer &buffer){
    glDeleteBuffers(1,&buffer.m_Handle.U32);
}

}//namespace GL::