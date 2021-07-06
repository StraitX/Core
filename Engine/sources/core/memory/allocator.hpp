#ifndef DEFAULT_ALLOCATOR_HPP
#define DEFAULT_ALLOCATOR_HPP

#include "platform/types.hpp"
#include "platform/compiler.hpp"
#include "platform/memory.hpp"

struct DefaultGeneralAllocator{
	void *Alloc(size_t size);

	void Free(void *ptr);
};

SX_INLINE void *DefaultGeneralAllocator::Alloc(size_t size){
	return Memory::Alloc(size);
}

SX_INLINE void DefaultGeneralAllocator::Free(void *ptr){
	Memory::Free(ptr);
}

class DefaultPoolAllocator{
private:
	size_t m_BlockSize;
	size_t m_BlockAlignment;
public:
	DefaultPoolAllocator(size_t block_size, size_t block_alignment);

	void *Alloc();

	void Free(void *ptr);
};

SX_INLINE DefaultPoolAllocator::DefaultPoolAllocator(size_t block_size, size_t block_alignment):
	m_BlockSize(block_size),
	m_BlockAlignment(block_alignment)
{}

SX_INLINE void *DefaultPoolAllocator::Alloc(){
	return Memory::AlignedAlloc(m_BlockSize, m_BlockAlignment);
}

SX_INLINE void DefaultPoolAllocator::Free(void *ptr){
	Memory::AlignedFree(ptr);
}

#endif //DEFAULT_ALLOCATOR_HPP