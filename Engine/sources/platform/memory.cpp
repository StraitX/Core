#include "platform/memory.hpp"
#include "platform/compiler.hpp"
#include "platform/io.hpp"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <assert.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

constexpr size_t s_DefaultAlignment =
max(
    max(
        max(
            max(alignof(char), alignof(wchar_t)),
            max(alignof(short), alignof(int))
        ),
        max(
            max(alignof(long), alignof(long long)),
            max(alignof(void*), alignof(ptrdiff_t))
        )
    ),
    max(
        max(
            max(alignof(clock_t), alignof(time_t)),
            max(alignof(size_t), alignof(long double))
        ),
        max(
            max(alignof(float), alignof(double)),
            0
        )
    )
);

#ifndef __STDCPP_DEFAULT_NEW_ALIGNMENT__
#define __STDCPP_DEFAULT_NEW_ALIGNMENT__ s_DefaultAlignment
#endif

void *operator new(size_t size, const std::nothrow_t&)noexcept{
	return StraitX::Memory::Alloc(size);
}

void operator delete(void* ptr, const std::nothrow_t&)noexcept{
	StraitX::Memory::Free(ptr);
}

void *operator new[](size_t size, const std::nothrow_t&)noexcept{
	return StraitX::Memory::Alloc(size);
}

void operator delete[](void* ptr, const std::nothrow_t&)noexcept{
	StraitX::Memory::Free(ptr);
}

void *operator new(size_t size){
	void *memory = ::operator new(size, std::nothrow);
	if(!memory)
		throw std::bad_alloc();
    return memory;
}

void operator delete(void *ptr)noexcept{
    ::operator delete(ptr, std::nothrow);
}

void *operator new[](size_t size){
    void *memory = ::operator new[](size, std::nothrow);
	if(!memory)
		throw std::bad_alloc();
    return memory;
}

void operator delete[](void *ptr)noexcept{
    return ::operator delete[](ptr, std::nothrow);
}

namespace StraitX{

static u64 s_Allocated = 0;
static u64 s_Freed = 0;
static u64 s_AllocCalls = 0;
static u64 s_FreeCalls = 0;

static constexpr size_t s_AllocationInfoSize = max(sizeof(size_t), __STDCPP_DEFAULT_NEW_ALIGNMENT__);

void *Memory::Alloc(size_t size){
#ifdef SX_DEBUG

    void *pointer = std::malloc(size + s_AllocationInfoSize);

	if(!pointer)
		return nullptr;
	
    s_Allocated += size;
    ++s_AllocCalls;

	pointer = (u8*)pointer + s_AllocationInfoSize;

	((size_t*)pointer)[-1] = size;

    return pointer;
#else
    return std::malloc(size);
#endif
}

void Memory::Free(void *pointer){
#ifdef SX_DEBUG
	if(!pointer)return;

    s_Freed += ((size_t*)pointer)[-1];
    ++s_FreeCalls;

    std::free((u8*)pointer - s_AllocationInfoSize);
#else
    std::free(pointer);
#endif
}

void *Memory::Realloc(void *pointer, size_t size){
#ifdef SX_DEBUG
    pointer = (u8*)pointer - s_AllocationInfoSize;

    void *new_pointer = std::realloc(pointer, size + s_AllocationInfoSize);

	if(!new_pointer)
		return nullptr;

    s_Freed += ((size_t*)pointer)[-1];
    ++s_FreeCalls;

    s_Allocated += size;
    ++s_AllocCalls;

	new_pointer = (u8*)new_pointer + s_AllocationInfoSize;

	((size_t*)new_pointer)[-1] = size;

    return new_pointer;
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