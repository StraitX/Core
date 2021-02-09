#include "platform/memory.hpp"
#include "core/assert.hpp"
#include "graphics/opengl/cpu_buffer_impl.hpp"

namespace StraitX{
namespace GL{

void CPUBufferImpl::NewImpl(CPUBuffer &buffer, size_t size){
    buffer.m_Size = size;
    buffer.m_Pointer = Memory::Alloc(size);
    CoreAssert(buffer.m_Pointer, "GL: CPUBufferImpl: can't allocate memory");
}

void CPUBufferImpl::DeleteImpl(CPUBuffer &buffer){
    Memory::Free(buffer.m_Pointer);
}

}//namespace GL::
}//namespace StraitX::
