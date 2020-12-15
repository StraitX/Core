#ifndef STRAITX_ARRAY_PTR_HPP
#define STRAITX_ARRAY_PTR_HPP 

#include "platform/types.hpp"
#include "core/assert.hpp"

namespace StraitX{

// it has no idea about allocations
template <typename T, typename SizeT = size_t>
struct ArrayPtr{
    T *Pointer;
    SizeT Size;

    sx_inline ArrayPtr():
        Pointer(nullptr),
        Size(0)
    {}

    sx_inline ArrayPtr(T *pointer, SizeT size):
        Pointer(pointer),
        Size(size)
    {}

    sx_inline ArrayPtr(const ArrayPtr &other):
        Pointer(other.Pointer),
        Size(other.Size)
    {}

    sx_inline ArrayPtr(ArrayPtr &&other):
        Pointer(other.Pointer),
        Size(other.size)
    {
        other.Pointer = nullptr;
        other.Size = 0;
    }

    sx_inline T &operator[](SizeT index){
        CoreAssert(index < Size && index >= 0, "ArrayPtr: can't index more that ArrayPtr::Size elements");
        return Pointer[index]; 
    }

    sx_inline const T &operator[](SizeT index)const{
        CoreAssert(index < Size && index >= 0, "ArrayPtr: can't index more that ArrayPtr::Size elements");
        return Pointer[index]; 
    }

    sx_inline ArrayPtr &operator=(const ArrayPtr &other){
        Pointer = other.Pointer;
        Size = other.Size;
    }

    sx_inline ArrayPtr &operator=(ArrayPtr &&other){
        Pointer = other.Pointer;
        Size = other.Size;
        other.Pointer = nullptr;
        other.Size = 0;
    }
    
    using iterator = T *;
    using const_iterator = const T *;

    sx_inline iterator begin(){
        return Pointer;
    }

    sx_inline iterator end(){
        return Pointer+Size;
    }

    sx_inline const_iterator begin()const{
        return Pointer;
    }

    sx_inline const_iterator end()const{
        return Pointer+Size;
    }

};

}; // namespace StraitX::

#endif //STRAITX_ARRAY_PTR_HPP