#ifndef STRAITX_MEMORY_HPP
#define STRAITX_MEMORY_HPP

#include "platform/types.hpp"

namespace StraitX{

class Memory{
public:
    static void *Alloc(size_t size);

    static void Free(void *pointer);

    static void *Realloc(void *pointer, size_t size);

};

}; // namespace StraitX::

#endif