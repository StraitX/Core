#ifndef STRAITX_IO_HPP
#define STRAITX_IO_HPP

#include "platform/error.hpp"
#include "platform/compiler.hpp"
#include "string.h"

namespace StraitX{


char *BufferPrint(char *buffer, const char *string);
char *BufferPrint(char *buffer, const char &ch);
char *BufferPrint(char *buffer, const int &num);
char *BufferPrint(char *buffer, const long &num);
char *BufferPrint(char *buffer, const long long &num);
char *BufferPrint(char *buffer, const unsigned int &num);
char *BufferPrint(char *buffer, const unsigned long &num);
char *BufferPrint(char *buffer, const unsigned long long &num);
char *BufferPrint(char *buffer, const float &num);
char *BufferPrint(char *buffer, const double &num);
char *BufferPrint(char *buffer, const void *ptr);

class Output{
public:
    static void Print(const char *string);

    static void Printf(const char *fmt);

    template<typename T, typename...Args>
    static void Printf(const char *fmt, T arg, Args...args){
        // i do not belive in buffer overflow
        char buffer[256];
        // just for now
        memset(buffer,0,256);
        SBufferPrint(buffer,fmt,arg,args...);
        Printf(buffer);
    }

    template <typename T, typename...Args>
    static void SBufferPrint(char *buffer, const char *fmt, T arg, Args...args){
        while(*fmt!=0){
            if(*fmt=='%'){
                return SBufferPrint(BufferPrint(buffer,arg),fmt+1,args...);
            }
            *buffer = *fmt;
            ++fmt;
            ++buffer;
        }
    }
    static void SBufferPrint(char *buffer, const char *fmt);
    
};

}; // namespace StraitX::

#endif //STRAITX_PLATFORM_HPP