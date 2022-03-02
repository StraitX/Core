#ifndef STRAITX_IO_HPP
#define STRAITX_IO_HPP

#include "core/types.hpp"
#include "core/printer.hpp"
#include "core/move.hpp"
#include "core/ranges.hpp"

void STDOutWriter(char ch, void *data = nullptr);

void STDErrWriter(char ch, void *data = nullptr);

void WriterPrint(void (*writer)(char, void*), void *writer_data, const char *fmt);

class StringView;
class String;

namespace Details{

template<typename Type>
struct ImplicitPrintCaster{
	static constexpr bool IsString = IsSame<String, Type>::Value || IsSame<StringView, Type>::Value;
	static constexpr bool IsTrueRange = IsRange<Type>::Value && !IsString;

	template<typename _Type = Type, typename = typename EnableIf<IsTrueRange>::Type>
	static auto Cast(const _Type& value) -> Range<decltype(Begin(value))>{
		return ToRange(value);
	}

	template<typename _Type = Type, typename = typename EnableIf<!IsTrueRange>::Type>
	static const Type& Cast(const _Type& value) {
		return value;
	}
};

template<typename T>
struct ImplicitPrintCaster<T *>{
	static void *Cast(T *value) {
		return static_cast<void*>(value);
	}
};

template<>
struct ImplicitPrintCaster<const char *>{
	static const char *Cast(const char *value) {
		return value;
	}
};

template<>
struct ImplicitPrintCaster<char *>{
	static const char* Cast(const char *value) {
		return value;
	}
};

template<typename T, size_t N>
struct ImplicitPrintCaster<T[N]>{
	static Range<const T*> Cast(const T value[N]) {
		return {value, value + N};
	}
};

template<size_t N>
struct ImplicitPrintCaster<char[N]>{
	static const char *Cast(const char value[N]) {
		return value;
	}
};

template<size_t N>
struct ImplicitPrintCaster<const char[N]>{
	static const char *Cast(const char value[N]) {
		return value;
	}
};

}//namespace Details::

template <typename T, typename...Args>
void WriterPrint(void (*writer)(char, void*), void *writer_data, const char *fmt, const T &arg, const Args&...args){
	while(*fmt!=0){
        if(*fmt=='%'){
			using CastResultType = decltype(Details::ImplicitPrintCaster<T>::Cast(Declval<T>()));
			using PrintType = typename RemoveConst<typename RemoveReference<CastResultType>::Type>::Type;
			Printer<PrintType>::Print(Details::ImplicitPrintCaster<T>::Cast(arg), writer, writer_data);
            return WriterPrint(writer, writer_data, fmt+1, ((const Args&)(args))...);
        }
        writer(*fmt, writer_data);
        ++fmt;
    }
	writer('\0', writer_data);
}

template<typename...Args>
void Print(const char *fmt, const Args&...args){
	WriterPrint(STDOutWriter, nullptr, fmt, args...);
}

template<typename...Args>
void Println(const char *fmt, const Args&...args){
    Print(fmt, args...);
    Print("\n");
}

#endif //STRAITX_PLATFORM_HPP