#ifndef STRAITX_IO_HPP
#define STRAITX_IO_HPP

#include "platform/types.hpp"

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
char *BufferPrint(char *buffer, const Version &version);

template <typename T, typename...Args>
void BufferPrint(char *buffer, const char *fmt, T arg,const Args&...args){
    while(*fmt!=0){
        if(*fmt=='%'){
            return (void)BufferPrint(BufferPrint(buffer,arg),fmt+1,args...);
        }
        *buffer = *fmt;
        ++fmt;
        ++buffer;
    }
    *buffer = 0;
}

void Print(const char *fmt);

template<typename T, typename...Args>
void Print(const char *fmt, T arg,const Args&...args){
    // i do not belive in buffer overflow
    char buffer[1024];
    BufferPrint(buffer,fmt,arg,args...);
    Print(buffer);
}

void Println(const char *fmt);

template<typename T, typename...Args>
void Println(const char *fmt, T arg,const Args&...args){
    Print(fmt, arg, args...);
    Print("\n");
}



}; // namespace StraitX::

#endif //STRAITX_PLATFORM_HPP