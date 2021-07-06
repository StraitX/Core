#ifndef STRAITX_RAW_VAR_HPP
#define STRAITX_RAW_VAR_HPP

#include "platform/types.hpp"
#include "platform/memory.hpp"
#include "core/move.hpp"
#include "core/noncopyable.hpp"

template<typename T>
class alignas(T) RawVar: public NonCopyable{
private:
	u8 m_Memory[sizeof(T)];
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

	T *operator&(){
		return Pointer();
	}

	const T *operator&()const{
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

#endif//STRAITX_RAW_VAR_HPP