#ifndef STRAITX_STACK_ALLOCATOR_HPP
#define STRAITX_STACK_ALLOCATOR_HPP

#include "platform/types.hpp"

namespace StraitX{

class StackAllocator{
private:
    u8 *m_Memory;
    size_t m_Capacity;
    size_t m_Size;
public:
    StackAllocator(size_t size);

    ~StackAllocator();

    void Finalize();

    void *Alloc(size_t size);

    void Free(size_t size);

    sx_inline void *Break(){
        return m_Memory+m_Size;
    }
    // Adds few unused offset bytes to align the allocation
    // therefore can't be reasonably freed
    void *AlignedAlloc(size_t size, size_t alignment);

};

};//namespace StraitX::

#endif //STRAITX_STACK_ALLOCATOR_HPP