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
    return ::operator new(size);
}
void operator delete[](void *ptr)noexcept{
    return ::operator delete(ptr);
}


namespace StraitX{

static u64 s_Allocated = 0;
static u64 s_Freed = 0;
static u64 s_AllocCalls = 0;
static u64 s_FreeCalls = 0;

#define max(a, b) (a > b ? a : b)
constexpr size_t s_DebugSizeSpace = max(sizeof(size_t), __STDCPP_DEFAULT_NEW_ALIGNMENT__);

void *Memory::Alloc(size_t size){
#ifdef SX_DEBUG
    s_Allocated += size;
    ++s_AllocCalls;

    void *memory = std::malloc(size + s_DebugSizeSpace);
    size_t *sized = (size_t*)memory;
    *sized = size;

    return (u8*)sized + s_DebugSizeSpace;
#else
    return std::malloc(size);
#endif
}
void Memory::Free(void *pointer){
#ifdef SX_DEBUG
    size_t *sized = (size_t*)((u8*)pointer - s_DebugSizeSpace);

    s_Freed += *sized;
    ++s_FreeCalls;

    std::free(sized);
#else
    std::free(pointer);
#endif
}
void *Memory::Realloc(void *pointer, size_t size){
#ifdef SX_DEBUG
    size_t *sized_to_free = (size_t*)((u8*)pointer - s_DebugSizeSpace);
    s_Freed += *sized_to_free;
    ++s_FreeCalls;

    s_Allocated += size;
    ++s_AllocCalls;

    void *memory = std::realloc(sized_to_free, size + s_DebugSizeSpace);

    size_t* sized_allocated = (size_t*)memory;
    *sized_allocated = size;

    return (u8*)sized_allocated + s_DebugSizeSpace;
#else
    return std::realloc(pointer,size);
#endif
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