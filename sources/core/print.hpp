#ifndef STRAITX_IO_HPP
#define STRAITX_IO_HPP

#include "core/types.hpp"
#include "core/printer.hpp"
#include "core/move.hpp"
#include "core/ranges.hpp"
#include "core/string_writer.hpp"

class StringView;
class String;

namespace Details{

template<typename Type>
struct ImplicitPrintCaster{
	template<typename CheckType>
	static constexpr bool IsString = IsSame<String, CheckType>::Value || IsSame<StringView, CheckType>::Value;
	template<typename CheckType>
	static constexpr bool IsTrueRange = IsRange<CheckType>::Value && !IsString<CheckType>;

	template<typename _Type = Type, typename = typename EnableIf<IsTrueRange<_Type>>::Type>
	static auto Cast(const _Type& value) -> Range<decltype(Begin(value))>{
		return ToRange(value);
	}

	template<typename _Type = Type, typename = typename EnableIf<!IsTrueRange<_Type>>::Type>
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

void WriterPrint(StringWriter &writer, const char *fmt);

template <typename T, typename...Args>
void WriterPrint(StringWriter &writer, const char *fmt, const T &arg, const Args&...args){
	while(*fmt!=0){
        if(*fmt=='%'){
			using CastResultType = decltype(Details::ImplicitPrintCaster<T>::Cast(Declval<T>()));
			using PrintType = typename RemoveConst<typename RemoveReference<CastResultType>::Type>::Type;
			Printer<PrintType>::Print(Details::ImplicitPrintCaster<T>::Cast(arg), writer);
            return WriterPrint(writer, fmt+1, ((const Args&)(args))...);
        }
		writer.Write(*fmt);
        ++fmt;
    }
	writer.Write('\0');
}

template <typename T, typename...Args>
void WriterPrint(StringWriter *writer, const char* fmt, const Args&...args) {
	return WriterPrint(*writer, fmt, args...);
}

template<typename...Args>
void Print(const char *fmt, const Args&...args){
	WriterPrint(*StraitXOut, fmt, args...);
}

template<typename...Args>
void Println(const char *fmt, const Args&...args){
    Print(fmt, args...); Print("\n");
}

template<typename...Args>
int Error(const char *fmt, const Args&...args){
	WriterPrint(*StraitXError, fmt, args...);
	return -1;
}

template<typename...Args>
int Errorln(const char *fmt, const Args&...args){
	return (Error(fmt, args...), Error("\n"));
}

#endif //STRAITX_PLATFORM_HPP