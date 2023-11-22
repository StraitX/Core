#ifndef STRAITX_PRINTER_HPP
#define STRAITX_PRINTER_HPP

#include "core/types.hpp"
#include "core/os/memory.hpp"
#include "core/string_writer.hpp"

template<typename T>
struct Printer{
	static void Print(const T &value, StringWriter &writer);
};

template<typename T>
class BufferPrinter{
private:
	struct BufferStringWriter {
		char* Buffer = nullptr;
		size_t Written = 0;

		BufferStringWriter(char *buffer):
			Buffer(buffer)
		{}
		void Write(const char *string, size_t size)override {
			Written += size;
			Memory::Copy(string, Buffer, size);
		}
	};

public:
	static size_t Print(const T &value, char *buffer){
		BufferStringWriter writer(buffer);
		Printer<T>::Print(value, writer);
		return writer.Written;
	}
};

#endif//STRAITX_PRINTER_HPP