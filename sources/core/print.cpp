#include "core/print.hpp"
#include "core/string_view.hpp"
#include <cstdio>

void WriterPrint(StringWriter &writer, const char *fmt){
	StringView string = fmt;
	writer.Write(string.Data(), string.Size());
}

void STDOutWriter(char ch, void*){
	putchar(ch);
}

void STDErrWriter(char ch, void *){
	fputc(ch, stderr);
	fflush(stderr);
}
