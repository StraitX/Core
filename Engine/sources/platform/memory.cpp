#include "platform/memory.hpp"
#include <cstdlib>
#include <cstring>
namespace StraitX{


void *Memory::Alloc(size_t size){
    return std::malloc(size);
}
void Memory::Free(void *pointer){
    std::free(pointer);
}
void *Memory::Realloc(void *pointer, size_t size){
    return std::realloc(pointer,size);
}

void Memory::Set(void *memory, char byte, size_t size){
    memset(memory,byte,size);
}

void Memory::Copy(const void *source, void *destination, size_t size){
    memcpy(destination,source,size);
}

}; // namespace StraitX::