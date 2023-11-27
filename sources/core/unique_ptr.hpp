#ifndef STRAITX_UNIQUE_PTR_HPP
#define STRAITX_UNIQUE_PTR_HPP

#include <core/move.hpp>
#include <core/type_traits.hpp>

template <typename Type>
struct DefaultDelete {
    constexpr DefaultDelete() noexcept = default;

    void operator()(Type* ptr) const noexcept {
        static_assert(0 < sizeof(Type), "can't delete an incomplete type");
        delete ptr;
    }
};

template <typename Type>
struct DefaultDelete<Type[]> {
    constexpr DefaultDelete() noexcept = default;

    void operator()(Type* ptr) const noexcept {
        static_assert(0 < sizeof(Type), "can't delete an incomplete type");
        delete[] ptr;
    }
};

template<typename Type, typename DeleterType = DefaultDelete<Type>>
class UniquePtr : private DeleterType{
private:
	Type *m_Ptr = nullptr;
public:
    UniquePtr() = default;
    
    template<typename DerivedType, typename = EnableIfType<IsBaseOf<Type, DerivedType>::Value>>
    UniquePtr(UniquePtr<DerivedType> derived) {
        m_Ptr = derived.Release();
    }

	UniquePtr(Type *ptr, DeleterType deleter = {}):
        DeleterType(Move(deleter)),
		m_Ptr(ptr)
	{}

	UniquePtr(const UniquePtr &) = delete;

    UniquePtr(UniquePtr&& other) {
        *this = Move(other);
    }

	~UniquePtr() {
        Reset();
	}

	UniquePtr &operator=(const UniquePtr &) = delete;

    UniquePtr &operator=(UniquePtr&& other) {
        Reset();
        DeleterType::operator=(Move(other));
        m_Ptr = other.m_Ptr;
        other.m_Ptr = nullptr;
        return *this;
    }

    Type *operator->() {
        return m_Ptr;
    }

    Type &operator*() {
        return *m_Ptr;
    }

    const Type *operator->()const {
        return m_Ptr;
    }

    const Type &operator*()const {
        return *m_Ptr;
    }

    Type* Get() {
        return m_Ptr;
    }

    const Type* Get()const {
        return m_Ptr;
    }

    operator bool()const {
        return m_Ptr;
    }

    [[nodiscard]] Type *Release() {
        auto *ptr = m_Ptr; 
        m_Ptr = nullptr;
        return ptr;
    }

    void Reset() {
		DeleterType::operator()(m_Ptr);
        m_Ptr = nullptr;
    }
};

template<typename Type, typename DeleterType>
class UniquePtr<Type[], DeleterType> : private DeleterType {
private:
    Type* m_Ptr = nullptr;
public:
    UniquePtr() = default;

	UniquePtr(Type *ptr, DeleterType deleter = {}):
        DeleterType(Move(deleter)),
		m_Ptr(ptr)
	{}

    UniquePtr(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) {
        *this = Move(other);
    }

    ~UniquePtr() {
        Reset();
    }

    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr& operator=(UniquePtr&& other) {
        Reset();
        DeleterType::operator=(Move(other));
        m_Ptr = other.m_Ptr;
        other.m_Ptr = nullptr;
        return *this;
    }

    Type* operator->() {
        return m_Ptr;
    }

    Type& operator*() {
        return *m_Ptr;
    }

    const Type* operator->()const {
        return m_Ptr;
    }

    const Type& operator*()const {
        return *m_Ptr;
    }

    operator bool()const {
        return m_Ptr;
    }
    
    Type* Get() {
        return m_Ptr;
    }

    const Type* Get()const{
        return m_Ptr;
    }

    [[nodiscard]] Type *Release() {
        auto *ptr = m_Ptr; 
        m_Ptr = nullptr;
        return ptr;
    }


    void Reset() {
		DeleterType::operator()(m_Ptr);
        m_Ptr = nullptr;
    }

    Type &operator[](size_t index) {
        return m_Ptr[index];
    }
    const Type &operator[](size_t index)const {
        return m_Ptr[index];
    }
};

template<typename Type, typename ...ArgsType>
UniquePtr<Type> MakeUnique(ArgsType&&...args) {
    return {new Type(Forward<ArgsType>(args)...)};
}

template<typename Type, typename ...ArgsType>
UniquePtr<Type[]> MakeUnique(size_t count) {
    return {new Type[count]{}};
}

#endif//STRAITX_UNIQUE_PTR_HPP