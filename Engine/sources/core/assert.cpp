#include "core/assert.hpp"
#include "platform/io.hpp"
#include <cstdlib>
#include "core/log.hpp"

void AssertFail(const char *assertion, const char *filename, unsigned int line, const char * function){
    char buffer[1024];
    StraitX::Output::BufferPrintf(buffer,"Assertion % failed\nfile: %\nline: %\nfunction: %\n",assertion,filename,line,function);
    LogError(buffer);
    exit(EXIT_FAILURE);
}


void CoreAssertFail(const char *assertion,const char *message, const char *function){
    char buffer[1024];
    StraitX::Output::BufferPrintf(buffer,"CoreAssertion % failed\nFrom function: %\nMessage: %\n",assertion,function,message);
    LogError(buffer);
    exit(EXIT_FAILURE);
}