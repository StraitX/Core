#include "core/allocators/linear_allocator.hpp"
#include "core/os/memory.hpp"
#include "core/assert.hpp"

LinearAllocator::LinearAllocator(size_t size):
    m_Memory(nullptr),
    m_Capacity(size),
    m_Size(0)
{
    SX_CORE_ASSERT(size != 0,"LinearAllocator: can't to create a ZERO sized allocator");
    m_Memory = (u8*)Memory::Alloc(size);
    m_Capacity = size;
    m_Size = 0;
    SX_CORE_ASSERT(m_Memory != nullptr, "LinearAllocator: Failed to allocate initial memory chunk");
}

LinearAllocator::~LinearAllocator(){
    Finalize();
}

LinearAllocator::LinearAllocator(LinearAllocator &&other):
    m_Memory(other.m_Memory),
    m_Capacity(other.m_Capacity),
    m_Size(other.m_Size)
{
    other.m_Memory = nullptr;
    other.m_Capacity = 0;
    other.m_Size = 0;
}

LinearAllocator &LinearAllocator::operator=(LinearAllocator &&other){
    SX_CORE_ASSERT(m_Memory == nullptr,"LinearAllocator: Can't move into non-Finalized instance");
    m_Memory = other.m_Memory;
    m_Capacity = other.m_Capacity;
    m_Size = other.m_Size;
    other.m_Memory = nullptr;
    other.m_Capacity = 0;
    other.m_Size = 0;
    return *this;
}

void LinearAllocator::Finalize(){
    Memory::Free(m_Memory);
    m_Memory = nullptr;
    m_Capacity = 0;
    m_Size = 0;
}

void *LinearAllocator::Alloc(size_t size){
    SX_CORE_ASSERT(m_Memory != nullptr, "LinearAllocator: can't allocate from the finalized allocator");
    SX_CORE_ASSERT(m_Capacity >= m_Size+size, "LinearAllocator::Alloc: Out of bounds");

    void *memory = Break();
    m_Size += size;

    return memory;
}

void LinearAllocator::Free(size_t size){
    SX_CORE_ASSERT(m_Size - size >= 0,"LinearAllocator: Overfree");
    m_Size -= size;
}

void *LinearAllocator::AlignedAlloc(size_t size, size_t alignment){
    SX_CORE_ASSERT(alignment > 1,"LinearAllocator: Don't use allignment of 1 and less, it is wasteful");

    size_t offset = (alignment - (ptr_t)Break() % alignment) % alignment;
    return (u8*)Alloc(size + offset) + offset;
}