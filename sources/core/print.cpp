#include "core/print.hpp"
#include "core/string.hpp"
#include <cstdio>

void WriterPrint(StringWriter &writer, const char *fmt){
	writer.Write(fmt, String::StaticCodeunitsCount(fmt));
}

void STDOutWriter(char ch, void*){
	putchar(ch);
}

void STDErrWriter(char ch, void *){
	fputc(ch, stderr);
	fflush(stderr);
}
