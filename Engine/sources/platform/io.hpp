#ifndef STRAITX_IO_HPP
#define STRAITX_IO_HPP

#include "platform/memory.hpp"

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

class Output{
public:
    static void Print(const char *string);

    static void Printf(const char *fmt);

    template<typename T, typename...Args>
    static void Printf(const char *fmt, T arg,const Args&...args){
        // i do not belive in buffer overflow
        char buffer[1024];
        Memory::Set(buffer,0,1024);
        BufferPrintf(buffer,fmt,arg,args...);
        Printf(buffer);
    }

    template <typename T, typename...Args>
    static void BufferPrintf(char *buffer, const char *fmt, T arg,const Args&...args){
        while(*fmt!=0){
            if(*fmt=='%'){
                return BufferPrintf(BufferPrint(buffer,arg),fmt+1,args...);
            }
            *buffer = *fmt;
            ++fmt;
            ++buffer;
        }
    }
    static void BufferPrintf(char *buffer, const char *fmt);
    
};

}; // namespace StraitX::

#endif //STRAITX_PLATFORM_HPP