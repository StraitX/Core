#ifndef STRAITX_OPTIONAL_HPP
#define STRAITX_OPTIONAL_HPP

#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/move.hpp"
#include <new>

template<typename Type>
class alignas(Type) Optional {
private:
	byte m_Memory[sizeof(Type)] = {};
	bool m_HasValue = false;
public:
	Optional() = default;

	Optional(const Type &value) {
		*this = value;	
	}

	Optional(Type &&value) {
		*this = value;
	}
	
	template<typename...ArgsType>
	Optional(ArgsType&&...args){
		Emplace(Forward<ArgsType>(args)...);
	}

	Optional(const Optional& other) {
		*this = other;
	}

	Optional(Optional&& other) {
		*this = Move(other);
	}

	~Optional() {
		Clear();
	}

	Optional& operator=(const Optional& other) {
		Clear();

		if (other.m_HasValue)
			new(ValuePointer()) Type(other.Value());
		m_HasValue = other.m_HasValue;
			
		return *this;
	}

	Optional& operator=(Optional&& other) {
		Clear();

		if (other.m_HasValue)
			new(ValuePointer()) Type(Move(other.Value()));
		m_HasValue = other.m_HasValue;

		return *this;
	}

	Optional &operator=(const Type& value) {
		Clear();

		new (ValuePointer()) Type(value);
		m_HasValue = true;

		return *this;
	}

	Optional &operator=(Type&& value) {
		Clear();

		new (ValuePointer()) Type(Move(value));
		m_HasValue = true;

		return *this;
	}
	
	template <typename ...ArgsType>
	void Emplace(ArgsType &&...args) {
		Clear();

		new(m_Memory) Type(Forward<ArgsType>(args)...);
		m_HasValue = true;
	}

	void Clear(){
		if(m_HasValue){
			ValuePointer()->~Type();
			m_HasValue = false;
		}
	}

	bool HasValue()const {
		return m_HasValue;
	}

	const Type &Value()const{
		SX_CORE_ASSERT(HasValue(), "Optional: There is no value");

		return *ValuePointer();
	}

	Type &Value() {
		return const_cast<Type&>(const_cast<const Optional<Type>* >(this)->Value());
	}

    operator Type&(){
        return Value();
    }

    operator const Type&()const{
        return Value();
    }

private:
	const Type *ValuePointer() const{
		return reinterpret_cast<const Type*>(m_Memory);
	}
	Type *ValuePointer(){
		return reinterpret_cast<Type*>(m_Memory);
	}
};

template<>
class Optional<void> {
public:
	Optional(void) = default;

	template<typename...ArgsType>
	Optional(ArgsType...args) {
		(void)0;
	}

	Optional(const Optional &) = default;

	Optional(Optional &&) = default;

	~Optional() = default;

	Optional &operator=(const Optional &) = default;

	Optional &operator=(Optional &&) = default;

	bool HasValue()const {
		return false;
	}
};

#endif//STRAITX_OPTIONAL_HPP