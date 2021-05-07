#include "platform/memory.hpp"
#include "platform/compiler.hpp"
#include "platform/io.hpp"
#include <cstdlib>
#include <cstring>


void *operator new(size_t size){
    return StraitX::Memory::Alloc(size);
}
void operator delete(void *ptr)noexcept{
    StraitX::Memory::Free(ptr);
}
void *operator new[](size_t size){
    return StraitX::Memory::Alloc(size);
}
void operator delete[](void *ptr)noexcept{
    StraitX::Memory::Free(ptr);
}

namespace StraitX{

static u64 s_Allocated = 0;
static u64 s_Freed = 0;
static u64 s_AllocCalls = 0;
static u64 s_FreeCalls = 0;

void *Memory::Alloc(size_t size){
#ifdef SX_DEBUG
    s_Allocated += size;
    ++s_AllocCalls;

    //Println("Allocated: %, InUse: %", size, InUse());
    auto *memory = std::malloc(size + sizeof(size_t));
    auto sized = (size_t*)memory;
    *sized = size;

    return sized + 1;
#else
    return std::malloc(size);
#endif
}
void Memory::Free(void *pointer){
#ifdef SX_DEBUG
    size_t *sized = (size_t*)pointer - 1;

    s_Freed += *sized;
    ++s_FreeCalls;

    //Println("Freed: %, InUse: %", *sized, InUse());

    std::free(sized);
#else
    std::free(pointer);
#endif
}
void *Memory::Realloc(void *pointer, size_t size){
    return std::realloc(pointer,size);
}

void Memory::Set(void *memory, u8 byte, size_t size){
    memset(memory,byte,size);
}

void Memory::Copy(const void *source, void *destination, size_t size){
    memcpy(destination,source,size);
}

u64 Memory::Allocated(){
    return s_Allocated;
}

u64 Memory::Freed(){
    return s_Freed;
}

u64 Memory::InUse(){
    return s_Allocated - s_Freed;
}

u64 Memory::AllocCalls(){
    return s_AllocCalls;
}

u64 Memory::FreeCalls(){
    return s_FreeCalls;
}


}; // namespace StraitX::