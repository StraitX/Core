#include "core/print.hpp"
#include <cstdio>

namespace StraitX{

void WriterPrint(void (*writer)(char, void*), void *writer_data, const char *fmt){
	while(*fmt)
		writer(*fmt++, writer_data);
}

void STDOutWriter(char ch, void*){
	putchar(ch);
}

void STDErrWriter(char ch, void *){
	fputc(ch, stderr);
}

void Print(const char *fmt){
    printf("%s",fmt);
}

void Println(const char *string){
    puts(string);
}


}; // namespace StraitX::