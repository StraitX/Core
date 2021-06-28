#ifndef STRAITX_IO_HPP
#define STRAITX_IO_HPP

#include "platform/types.hpp"
#include "core/printer.hpp"

namespace StraitX{

void STDOutWriter(char ch, void *data = nullptr);

void STDErrWriter(char ch, void *data = nullptr);

void WriterPrint(void (*writer)(char, void*), void *writer_data, const char *fmt);

namespace Details{

template<typename T>
struct ImplicitPrintCaster{
	typedef T Type;
};

template<typename T>
struct ImplicitPrintCaster<T *>{
	typedef void * Type;
};

template<>
struct ImplicitPrintCaster<const char *>{
	typedef const char *Type;
};

}//namespace Details::

template <typename T, typename...Args>
void WriterPrint(void (*writer)(char, void*), void *writer_data, const char *fmt, T arg, const Args&...args){
	while(*fmt!=0){
        if(*fmt=='%'){
			using PrintType = typename Details::ImplicitPrintCaster<T>::Type;
			Printer<PrintType>::Print((PrintType)arg, writer, writer_data);
            return WriterPrint(writer, writer_data, fmt+1, args...);
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



}; // namespace StraitX::

#endif //STRAITX_PLATFORM_HPP