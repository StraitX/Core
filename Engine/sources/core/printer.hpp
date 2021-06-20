#ifndef STRAITX_PRINTER_HPP
#define STRAITX_PRINTER_HPP

#include "platform/types.hpp"

namespace StraitX{


template<typename T>
struct Printer{
	static void Print(const T &value, void (*writer)(char, void*), void *writer_data);
};

template<typename T>
class BufferPrinter{
private:
	struct BufferPrinterState{
		size_t Written;
		char *Buffer;
	};

	static void Writer(char ch, void *data){
		auto state = (BufferPrinterState*)data;
		state->Buffer[state->Written++] = ch;
	}
public:
	static size_t Print(const T &value, char *buffer){
		BufferPrinterState state;
		state.Written = 0;
		state.Buffer = buffer;
		Printer<T>::Print(value, Writer, &state);
		return state.Written;
	}
};

}//namespace StraitX::

#endif//STRAITX_PRINTER_HPP