#include "core/memory/stack_allocator.hpp"
#include "platform/memory.hpp"
#include "core/assert.hpp"

namespace StraitX{

StackAllocator::StackAllocator(size_t size):
    m_Memory(nullptr),
    m_Capacity(size),
    m_Size(0)
{
    CoreAssert(size != 0,"StackAllocator: can't to create a ZERO sized allocator");
    m_Memory = (u8*)Memory::Alloc(size);
    m_Capacity = size;
    m_Size = 0;
    CoreAssert(m_Memory != nullptr, "StackAllocator: Failed to allocate initial memory chunk");
}

StackAllocator::~StackAllocator(){
    Finalize();
}

StackAllocator::StackAllocator(StackAllocator &&other):
    m_Memory(other.m_Memory),
    m_Capacity(other.m_Capacity),
    m_Size(other.m_Size)
{
    other.m_Memory = nullptr;
    other.m_Capacity = 0;
    other.m_Size = 0;
}

StackAllocator &StackAllocator::operator=(StackAllocator &&other){
    CoreAssert(m_Memory == nullptr,"StackAllocator: Can't move into non-Finalized instance");
    m_Memory = other.m_Memory;
    m_Capacity = other.m_Capacity;
    m_Size = other.m_Size;
    other.m_Memory = nullptr;
    other.m_Capacity = 0;
    other.m_Size = 0;
    return *this;
}

void StackAllocator::Finalize(){
    Memory::Free(m_Memory);
    m_Memory = nullptr;
    m_Capacity = 0;
    m_Size = 0;
}

void *StackAllocator::Alloc(size_t size){
    CoreAssert(m_Memory != nullptr, "StackAllocator: can't allocate from the finalized allocator");
    CoreAssert(m_Capacity >= m_Size+size, "StackAllocator::Alloc: Out of bounds");

    void *memory = Break();
    m_Size += size;

    return memory;
}

void StackAllocator::Free(size_t size){
    CoreAssert(m_Size - size >= 0,"StackAllocator: Overfree");
    m_Size -= size;
}
void *StackAllocator::AlignedAlloc(size_t size, size_t alignment){
    CoreAssert(alignment > 1,"StackAllocator: Don't use allignment of 1 and less, it is wasteful");

    size_t offset = (alignment - (ptr_t)Break() % alignment) % alignment;
    return (u8*)Alloc(size + offset) + offset;
}


};//namespace StraitX::