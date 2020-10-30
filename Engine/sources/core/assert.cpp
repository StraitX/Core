#include "core/assert.hpp"
#include "platform/io.hpp"
#include <cstdlib>

void AssertFail(const char *assertion, const char *filename, unsigned int line, const char * function){
    StraitX::Output::Printf("[ERROR]: Assertion % failed\nfile: %\nline: %\nfunction: %\n",assertion,filename,line,function);
    exit(EXIT_FAILURE);
}


void CoreAssertFail(const char *assertion,const char *message, const char *function){
    StraitX::Output::Printf("[ERROR]: CoreAssertion % failed\nFrom function: %\nMessage: %\n",assertion,function,message);
    exit(EXIT_FAILURE);
}