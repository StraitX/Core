#ifndef STRAITX_RING_HPP
#define STRAITX_RING_HPP

#include "core/types.hpp"
#include "core/move.hpp"
#include "core/noncopyable.hpp"

template<typename Type, size_t SizeValue>
struct Ring: public NonCopyable{
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
private:
    Type m_Array[SizeValue] = {};
    size_t m_CurrentIndex = 0;
public:

    template<typename ...ArgsType>
    Ring(ArgsType&...args):
        m_Array{Forward<ArgsType>(args)...} 
    {}

    ~Ring() = default;

    void Advance(){
        m_CurrentIndex = WrapIndex(m_CurrentIndex + 1);
    }

    Type &Current(){
        return m_Array[m_CurrentIndex];
    }
    
    const Type &Current()const{
        return m_Array[m_CurrentIndex];
    }

    Type &Next(){
        return m_Array[WrapIndex(m_CurrentIndex + 1)];
    }
    
    const Type &Next()const{
        return m_Array[WrapIndex(m_CurrentIndex + 1)];
    }

    Type &operator[](size_t index){
        return m_Array[WrapIndex(index)];
    }

    const Type &operator[](size_t index)const{
        return m_Array[WrapIndex(index)];
    }

    size_t Size()const{
        return SizeValue;
    }

    Type *Data(){
        return m_Array;
    }

    const Type *Data()const{
        return m_Array;
    }

    Iterator begin(){
        return Data();
    }

    Iterator end(){
        return Data() + Size();
    }


    ConstIterator begin()const{
        return Data();
    }

    ConstIterator end()const{
        return Data() + Size();
    }
private:

    size_t WrapIndex(size_t index){
        return index % SizeValue;
    }
};

#endif//STRAITX_RING_HPP