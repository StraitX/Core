#ifndef STRAITX_LINEAT_ALLOCATOR_HPP
#define STRAITX_LINEAT_ALLOCATOR_HPP

#include "platform/types.hpp"

class LinearAllocator{
private:
    u8 *m_Memory;
    size_t m_Capacity;
    size_t m_Size;
public:
    LinearAllocator(size_t size);

    ~LinearAllocator();

    LinearAllocator(const LinearAllocator &other) = delete;

    LinearAllocator(LinearAllocator &&other);

    LinearAllocator &operator=(const LinearAllocator &other) = delete;

    LinearAllocator &operator=(LinearAllocator &&other);

    void Finalize();

    void *Alloc(size_t size);

    void Free(size_t size);

    SX_INLINE void *Break(){
        return m_Memory+m_Size;
    }
    // Adds few unused offset bytes to align the allocation
    // therefore can't be reasonably freed
    void *AlignedAlloc(size_t size, size_t alignment);

};

#endif //STRAITX_LINEAT_ALLOCATOR_HPP