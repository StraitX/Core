#ifndef STRAITX_RAW_VAR_HPP
#define STRAITX_RAW_VAR_HPP

#include "core/types.hpp"
#include "core/os/memory.hpp"
#include "core/move.hpp"
#include "core/noncopyable.hpp"
#include "core/printer.hpp"

template<typename T>
class alignas(T) RawVar: public NonCopyable{
private:
	byte m_Memory[sizeof(T)];
public:
	RawVar() = default;

	~RawVar() = default;

	template<typename ...Args>
	void Construct(Args&&...args){
		new (m_Memory) T(Forward<Args>(args)...);
	}

	void Destruct(){
		Pointer()->~T();
	}

	operator T*(){
		return Pointer();
	}

	operator const T*()const{
		return Pointer();	
	}

	T *operator->(){
		return Pointer();
	}

	const T *operator->()const{
		return Pointer();
	}

	T &operator*(){
		return *Pointer();
	}

	const T &operator*()const{
		return *Pointer();
	}

	T *Pointer(){
		return reinterpret_cast<T *>(m_Memory);
	}

	const T *Pointer()const{
		return reinterpret_cast<const T *>(m_Memory);
	}
};

template<typename T>
struct Printer<RawVar<T>> {
	static void Print(const RawVar<T>& value, StringWriter &writer) {
		Printer<T>::Print(*value.Pointer(), writer);
	}
};

#endif//STRAITX_RAW_VAR_HPP