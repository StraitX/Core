#ifndef STRAITX_UNIQUE_PTR_HPP
#define STRAITX_UNIQUE_PTR_HPP

#include <core/move.hpp>

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
	UniquePtr(Type *ptr):
		m_Ptr(ptr)
	{}

	UniquePtr(const UniquePtr &) = delete;

    UniquePtr(UniquePtr&& other) {
        *this = Move(other);
    }

	~UniquePtr() {
		DeleterType::operator()(m_Ptr);
	}

	UniquePtr &operator=(const UniquePtr &) = delete;

    UniquePtr &operator=(UniquePtr&& other) {
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
};

template<typename Type, typename DeleterType>
class UniquePtr<Type[], DeleterType> : private DeleterType {
private:
    Type* m_Ptr = nullptr;
public:
    UniquePtr(Type* ptr) :
        m_Ptr(ptr)
    {}

    UniquePtr(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) {
        *this = Move(other);
    }

    ~UniquePtr() {
        DeleterType::operator()(m_Ptr);
    }

    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr& operator=(UniquePtr&& other) {
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

    Type &operator[](size_t index) {
        return m_Ptr[index];
    }
    const Type &operator[](size_t index)const {
        return m_Ptr[index];
    }
};

#endif//STRAITX_UNIQUE_PTR_HPP