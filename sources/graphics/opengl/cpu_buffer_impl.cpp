#include "core/os/memory.hpp"
#include "core/assert.hpp"
#include "graphics/opengl/cpu_buffer_impl.hpp"

namespace GL{

static u64 s_CPUBufferId = 0;

void CPUBufferImpl::NewImpl(CPUBuffer &buffer, u32 size){
    buffer.m_Size = size;
    buffer.m_Pointer = Memory::Alloc(size);
    buffer.m_Handle.U64 = ++s_CPUBufferId;
    SX_CORE_ASSERT(buffer.m_Pointer, "GL: CPUBufferImpl: can't allocate memory");
}

void CPUBufferImpl::DeleteImpl(CPUBuffer &buffer){
    Memory::Free(buffer.m_Pointer);
}

}//namespace GL::