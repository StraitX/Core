#include "core/print.hpp"
#include <cstdio>

void WriterPrint(void (*writer)(char, void*), void *writer_data, const char *fmt){
	while(*fmt)
		writer(*fmt++, writer_data);
	writer('\0', writer_data);
}

void STDOutWriter(char ch, void*){
	putchar(ch);
}

void STDErrWriter(char ch, void *){
	fputc(ch, stderr);
	fflush(stderr);
}
