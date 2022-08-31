#include "core/assert.hpp"
#include "core/print.hpp"
#include "core/os/memory.hpp"
#include <cstdlib>

void _AssertFail(const char *assertion, const char *filename, unsigned int line, const char * function){
    char buffer[1024];
    WriterPrint(*StraitXError, buffer,"Assertion % failed\nfile: %\nline: %\nfunction: %\n", assertion, filename, line, function);
    abort();
}
void _AssertFail(const char *assertion, const char *message, const char *filename, unsigned int line, const char * function){
    char buffer[1024];
    WriterPrint(*StraitXError, "Assertion % failed\nmessage: %\nfile: %\nline: %\nfunction: %\n",assertion, message, filename, line, function);
    abort();
}

void _CoreAssertFail(const char *assertion, const char *message, const char * function){
    char buffer[1024];
    WriterPrint(*StraitXError, "CoreAssertion % failed\nFrom function: %\nMessage: %\n",assertion,function,message);
    abort();
}
