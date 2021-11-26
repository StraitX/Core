#ifndef STRAITX_ARRAY_HPP
#define STRAITX_ARRAY_HPP

#include <initializer_list>
#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/span.hpp"

template<typename Type, size_t SizeValue>
class Array{
private:
    template<typename StorageType, size_t StorageSizeValue>
    struct ArrayStorage{
        StorageType m_Data[StorageSizeValue];

        StorageType *Data()const{
            return const_cast<StorageType*>(m_Data);
        }
    };

    template<typename StorageType>
    struct ArrayStorage<StorageType, 0>{ 
        StorageType *Data()const{
            return nullptr;
        }
    };
public:
    using Iterator = Type *;
    using ConstIterator = const Type *;
public:
    ArrayStorage<Type, SizeValue> _Elements;
public:
    Array() = default;

    Array(const Array &other) = default;

    Array(Array &&other) = default;

    ~Array() = default;

    Array &operator=(const Array &other) = default;

    Array &operator=(Array &&other) = default;

    const Type &operator[](size_t index)const{
        SX_CORE_ASSERT(index < SizeValue, "Array: Index out of range");

        return _Elements.Data()[index];
    }

    Type &operator[](size_t index){
        return const_cast<Type&>(const_cast<const Array<Type, SizeValue>*>(this)->operator[](index));
    }

    operator Span<Type>(){
        return {Data(), Size()};
    }

    operator ConstSpan<Type>()const{
        return {Data(), Size()};
    }

    size_t Size()const{
        return SizeValue;
    }

    const Type *Data()const{
        return _Elements.Data();
    }

    Type *Data(){
        return _Elements.Data();
    }

    Type &First(){
        return operator[](0);
    }

    const Type &First()const{
        return operator[](0);
    }

    Type &Last(){
        return operator[](Size() - 1);
    }

    const Type &Last()const{
        return operator[](Size() - 1);
    }


    ConstIterator begin()const{
        return Data();
    }

    ConstIterator end()const{
        return Data() + Size();
    }

    Iterator begin(){
        return Data();
    }

    Iterator end(){
        return Data() + Size();
    }
};

#endif//STRAITX_ARRAY_HPP