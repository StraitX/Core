#include "core/assert.hpp"
#include "platform/io.hpp"
#include "platform/memory.hpp"
#include <cstdlib>
#include "core/log.hpp"


namespace StraitX{

void _AssertFail(const char *assertion, const char *filename, unsigned int line, const char * function){
    char buffer[1024];
    StraitX::BufferPrint(buffer,"Assertion % failed\nfile: %\nline: %\nfunction: %\n", assertion, filename, line, function);
    LogError(buffer);
    exit(EXIT_FAILURE);
}
void _AssertFail(const char *assertion, const char *message, const char *filename, unsigned int line, const char * function){
    char buffer[1024];
    StraitX::BufferPrint(buffer,"Assertion % failed\nmessage: %\nfile: %\nline: %\nfunction: %\n",assertion, message, filename, line, function);
    LogError(buffer);
    exit(EXIT_FAILURE);
}

void _CoreAssertFail(const char *assertion, const char *message, const char * function){
    char buffer[1024];
    StraitX::BufferPrint(buffer,"CoreAssertion % failed\nFrom function: %\nMessage: %\n",assertion,function,message);
    LogError(buffer);
    exit(EXIT_FAILURE);
}

}//namespace StraitX: