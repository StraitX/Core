#include "core/assert.hpp"
#include "core/print.hpp"
#include "core/os/memory.hpp"
#include "core/unicode.hpp"
#include <cstdlib>

void _AssertFail(const char *assertion, const char *filename, unsigned int line, const char * function){
    char buffer[1024];
    WriterPrint(*StraitXError, buffer, SX_UTF8("Assertion % failed\nfile: %\nline: %\nfunction: %\n"), assertion, filename, line, function);
    abort();
}
void _AssertFail(const char *assertion, const char *message, const char *filename, unsigned int line, const char * function){
    char buffer[1024];
    WriterPrint(*StraitXError, SX_UTF8("Assertion % failed\nmessage: %\nfile: %\nline: %\nfunction: %\n"),assertion, message, filename, line, function);
    abort();
}

void _CoreAssertFail(const char *assertion, const char *message, const char * function){
    char buffer[1024];
    WriterPrint(*StraitXError, SX_UTF8("CoreAssertion % failed\nFrom function: %\nMessage: %\n"),assertion,function,message);
    abort();
}
