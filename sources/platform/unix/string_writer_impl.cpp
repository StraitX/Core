#include "core/string_writer.hpp"
#include "core/os/memory.hpp"
#include <cstdio>
#include <iostream>

struct OutStringWriter: StringWriter{
	void Write(const char *buffer, size_t size)override{
		std::cout.write(buffer, size);
	}
};

struct ErrorStringWriter: StringWriter{
	void Write(const char *buffer, size_t size)override{
		std::cerr.write(buffer, size);
	}
};

static OutStringWriter WindowsStraitXOut;
static ErrorStringWriter WindowsStraitXError;

StringWriter* StraitXOut = &WindowsStraitXOut;
StringWriter* StraitXError = &WindowsStraitXError;