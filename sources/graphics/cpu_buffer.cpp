#include "platform/memory.hpp"
#include "graphics/cpu_buffer.hpp"

CPUBuffer::VTable CPUBuffer::s_VTable;

void CPUBuffer::CopyData(const void *source, size_t size){
    SX_CORE_ASSERT(size <= m_Size, "CPUBuffer: CopyData: data size is bigger than buffer capacity");
    Memory::Copy(source, Pointer(), size);
}