#ifndef STRAITX_IO_HPP
#define STRAITX_IO_HPP

#include "platform/types.hpp"
#include "core/printer.hpp"

namespace StraitX{

void STDOutWriter(char ch, void *data = nullptr);

void STDErrWriter(char ch, void *data = nullptr);

void WriterPrint(void (*writer)(char, void*), void *writer_data, const char *fmt);

template <typename T, typename...Args>
void WriterPrint(void (*writer)(char, void*), void *writer_data, const char *fmt, T arg, const Args&...args){
	while(*fmt!=0){
        if(*fmt=='%'){
			Printer<T>::Print(arg, writer, writer_data);
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