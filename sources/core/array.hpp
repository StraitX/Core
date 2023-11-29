#ifndef STRAITX_ARRAY_HPP
#define STRAITX_ARRAY_HPP

#include <initializer_list>
#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/span.hpp"
#include "core/mixins.hpp"

template<typename Type, size_t SizeValue>
class Array: public ArrayMixin<Array<Type, SizeValue>, Type>{
private:
    template<typename StorageType, size_t StorageSizeValue>
    struct ArrayStorage{
        StorageType m_Data[StorageSizeValue];

        constexpr StorageType *Data()const{
            return const_cast<StorageType*>(m_Data);
        }
    };

    template<typename StorageType>
    struct ArrayStorage<StorageType, 0>{ 
        constexpr StorageType *Data()const{
            return nullptr;
        }
    };
public:
    using Iterator = Type *;
    using ConstIterator = const Type *;
public:
    ArrayStorage<Type, SizeValue> _Elements;
public:
    constexpr Array() = default;

    constexpr Array(const Array &other) = default;

    constexpr Array(Array &&other) = default;

    ~Array() = default;

    constexpr Array &operator=(const Array &other) = default;

    constexpr Array &operator=(Array &&other) = default;

    const Type &operator[](size_t index)const{
        return this->At(index);
    }

    Type &operator[](size_t index){
        return this->At(index);
    }

    constexpr size_t Size()const{
        return SizeValue;
    }

    constexpr const Type *Data()const{
        return _Elements.Data();
    }

    constexpr Type *Data(){
        return _Elements.Data();
    }
};

#endif//STRAITX_ARRAY_HPP