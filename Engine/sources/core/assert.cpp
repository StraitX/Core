#include "core/assert.hpp"

#include <cstdio>
#include <cstdlib>

void AssertFail(const char *assertion, const char *filename, unsigned int line, const char * function){
    printf("[ERROR]: Assertion %s failed\nfile: %s\nline: %u\nfunction: %s\n",assertion,filename,line,function);
    exit(EXIT_FAILURE);
}


void CoreAssertFail(const char *assertion,const char *message, const char *function){
    printf("[ERROR]: CoreAssertion %s failed\nFrom function: %s\nMessage: %s\n",assertion,function,message);
    exit(EXIT_FAILURE);
}