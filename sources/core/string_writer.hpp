#ifndef STRAITX_STRING_WRITER_HPP
#define STRAITX_STRING_WRITER_HPP

#include "core/unicode.hpp"

struct StringWriter {
	virtual void Write(const char *string, size_t size) = 0;

	void Write(u32 utf32) {
		char utf8[5] = { 0 };
		u8 size = Codepoint(&utf32).Encode(utf8);
		Write(utf8, size);
	}
};

//Defined in platform layer
extern StringWriter* StraitXOut;
extern StringWriter* StraitXError;

#endif//STRAITX_STRING_WRITER_HPP