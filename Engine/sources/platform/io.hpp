#ifndef STRAITX_IO_HPP
#define STRAITX_IO_HPP

#include "platform/error.hpp"
#include "platform/compiler.hpp"
#include "string.h"

namespace StraitX{


char *VPrintf(char *buffer, const char *string);
char *VPrintf(char *buffer, const char &ch);
char *VPrintf(char *buffer, const int &num);
char *VPrintf(char *buffer, const long &num);
char *VPrintf(char *buffer, const long long &num);
char *VPrintf(char *buffer, const float &num);
char *VPrintf(char *buffer, const double &num);
char *VPrintf(char *buffer, const void *ptr);

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
        SVPrintf(buffer,fmt,arg,args...);
        Printf(buffer);
    }

    template <typename T, typename...Args>
    static void SVPrintf(char *buffer, const char *fmt, T arg, Args...args){
        while(*fmt!=0){
            if(*fmt=='%'){
                return SVPrintf(VPrintf(buffer,arg),fmt+1,args...);
            }
            *buffer = *fmt;
            ++fmt;
            ++buffer;
        }
    }
    static void SVPrintf(char *buffer, const char *fmt);
    
};

}; // namespace StraitX::

#endif //STRAITX_PLATFORM_HPP