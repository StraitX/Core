#include "platform/memory.hpp"
#include "graphics/api/cpu_buffer.hpp"

namespace StraitX{

CPUBuffer::VTable CPUBuffer::s_VTable;

void CPUBuffer::CopyData(const void *source, size_t size){
    SX_CORE_ASSERT(size <= m_Size, "CPUBuffer: CopyData: data size is bigger than buffer capacity");
    Memory::Copy(source, Pointer(), size);
}

}//namespace StraitX::