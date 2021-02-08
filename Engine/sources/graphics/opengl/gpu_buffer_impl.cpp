#include "platform/opengl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"

namespace StraitX{
namespace GL{

void GPUBufferImpl::NewImpl(GPUBuffer &buffer, LogicalGPU &owner, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage){
    buffer.m_Owner = &owner;
    buffer.m_Size = size;
    (void)usage;
    
    glGenBuffers(1, &buffer.m_Handle.U32);
    glBindBuffer(GL_COPY_WRITE_BUFFER, buffer.m_Handle.U32);

    glBufferData(GL_COPY_WRITE_BUFFER, buffer.m_Size, nullptr, mem_type == GPUMemoryType::VRAM ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
}

void GPUBufferImpl::DeleteImpl(GPUBuffer &buffer){
    glDeleteBuffers(1,&buffer.m_Handle.U32);
}

}//namespace GL::
}//namespace StraitX::
