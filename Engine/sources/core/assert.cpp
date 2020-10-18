#include "core/assert.hpp"

#include <cstdio>
#include <cstdlib>

void assert_fail(const char *assertion, const char *filename, unsigned int line, const char * function){
    printf("[ERROR]: Assertion %s failed\nfile: %s\nline: %u\nfunction: %s\n",assertion,filename,line,function);
    exit(EXIT_FAILURE);
}

