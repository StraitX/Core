#ifndef STRAITX_ARRAY_HPP
#define STRAITX_ARRAY_HPP

#include <initializer_list>
#include "core/types.hpp"
#include "core/assert.hpp"

template<typename Type, size_t SizeValue>
class Array{
private:
    template<typename StorageType, size_t StorageSizeValue>
    class ArrayStorage{
    private:
        StorageType m_Data[StorageSizeValue];
    public:
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
private:
    ArrayStorage<Type, SizeValue> m_Elements;
public:
    Array() = default;

    Array(std::initializer_list<Type> list){
        SX_CORE_ASSERT(list.size() <= SizeValue, "Array: initializer list exceeding array's size");

        for(size_t i = 0; i<list.size(); i++){
            (*this)[i] = list.begin()[i];
        }
    }

    Array(const Array &other) = default;

    Array(Array &&other) = default;

    ~Array() = default;

    Array &operator=(const Array &other) = default;

    Array &operator=(Array &&other) = default;

    const Type &operator[](size_t index)const{
        SX_CORE_ASSERT(index < SizeValue, "Array: Index out of range");

        return m_Elements.Data()[index];
    }

    Type &operator[](size_t index){
        return const_cast<Type&>(const_cast<const Array<Type, SizeValue>*>(this)->operator[](index));
    }

    size_t Size()const{
        return SizeValue;
    }

    const Type *Data()const{
        return m_Elements.Data();
    }

    Type *Data(){
        return m_Elements.Data();
    }

    const Type *begin()const{
        return Data();
    }

    const Type *end()const{
        return Data() + Size();
    }

    Type *begin(){
        return Data();
    }

    Type *end(){
        return Data() + Size();
    }
};

#endif//STRAITX_ARRAY_HPP