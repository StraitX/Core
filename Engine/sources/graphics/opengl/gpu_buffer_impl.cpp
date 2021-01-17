#include "platform/opengl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"

namespace StraitX{
namespace GL{

Result GPUBufferImpl::Create(GPUBuffer &buffer, LogicalGPU &owner, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage){
    buffer.m_Owner = &owner;
    buffer.m_Size = size;
    buffer.m_BackingMemory.U64 = 0;
    buffer.m_Handle.U64 = 0; // to make it clean, will be initialized during glGenBuffers
    (void)usage;
    
    glGenBuffers(1, &buffer.m_Handle.U32);
    glBindBuffer(GL_COPY_WRITE_BUFFER, buffer.m_Handle.U32);

    glBufferData(GL_COPY_WRITE_BUFFER, buffer.m_Size, nullptr, mem_type == GPUMemoryType::VRAM ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    return Result::Success;
}

void GPUBufferImpl::Destroy(GPUBuffer &buffer){
    glDeleteBuffers(1,&buffer.m_Handle.U32);

    buffer.m_Owner = nullptr;
    buffer.m_Size = 0;
    buffer.m_BackingMemory.U64 = 0;
    buffer.m_Handle.U64 = 0;
}

}//namespace GL::
}//namespace StraitX::
