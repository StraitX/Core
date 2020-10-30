#include "platform/memory.hpp"
#include <cstdlib>
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


}; // namespace StraitX::