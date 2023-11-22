#ifndef STRAITX_SPAN_HPP
#define STRAITX_SPAN_HPP 

#include <initializer_list>
#include "core/types.hpp"
#include "core/type_traits.hpp"
#include "core/templates.hpp"
#include "core/assert.hpp"

// it has no idea about allocations, object lifetime and stuff
template <typename Type, typename SizeType = size_t>
class Span{
public:
    using Iterator = Type*;
private:
    Type *m_Pointer = nullptr;
    SizeType m_Size = 0;
public:

    constexpr Span() = default;

    constexpr Span(const Span &other) = default;

    constexpr Span(Type &element):
        Span(&element, 1)
    {}

    constexpr Span(Type *pointer, SizeType size):
        m_Pointer(pointer),
        m_Size(size)
    {}
    
    template<typename _ = typename EnableIf<IsConst<Type>::Value, void>::Type>
    constexpr Span(std::initializer_list<Type> list):
        m_Pointer(list.begin()),
        m_Size(static_cast<SizeType>(list.size()))
    {}

    template<typename _Type = Type, 
             typename NonConstType = typename RemoveConst<_Type>::Type,
             EnableIfType<IsConst<_Type>::Value, bool> = true>
    constexpr Span(NonConstType *pointer, SizeType size):
        m_Pointer(pointer),
        m_Size(size)
    {}

    template<typename _Type = Type, 
             typename NonConstType = typename RemoveConst<_Type>::Type,
             EnableIfType<IsConst<_Type>::Value, bool> = true>
    constexpr Span(Span<NonConstType> span):
        m_Pointer(span.Pointer()),
        m_Size(span.Size())
    {}


    template<size_t ArraySize>
    constexpr Span(Type (&array)[ArraySize]):
        m_Pointer(&array[0]),
        m_Size(ArraySize)
    {}

    template<size_t ArraySize,
             typename _Type = Type,
             typename NonConstType = typename RemoveConst<_Type>::Type,
             EnableIfType<IsConst<_Type>::Value, bool> = true>
    constexpr Span(NonConstType (&array)[ArraySize]):
        m_Pointer(&array[0]),
        m_Size(ArraySize)
    {}

    constexpr Type &operator[](SizeType index)const{
        SX_CORE_ASSERT(IsValidIndex(index), "Span: can't index more that Span::Size elements");
        return m_Pointer[index]; 
    }

    constexpr Span &operator=(const Span &other) = default;

    constexpr operator Span<const Type>()const{
        return {Pointer(), Size()};
    }

    constexpr bool IsValidIndex(size_t index)const{
        return index < Size();
    }

    constexpr SizeType Size()const{
        return m_Size;
    }
    // XXX Change it to Data()
    constexpr Type *Pointer()const{
        return m_Pointer;
    }

    constexpr Type &First(){
        return operator[](0);
    }

    constexpr Type &Last(){
        return operator[](Size() - 1);
    }

    constexpr Iterator begin()const{
        return Pointer();
    }

    constexpr Iterator end()const{
        return Pointer() + Size();
    }
};

template <typename Type, typename SizeType = size_t>
using ConstSpan = Span<const Type, SizeType>;

#endif //STRAITX_SPAN_HPP