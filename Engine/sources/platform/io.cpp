#include "platform/io.hpp"
#include <iostream>

namespace StraitX{


char *BufferPrint(char *buffer,const char *string){
    while(*string!=0){
        *buffer=*string;
        ++buffer;
        ++string;
    }
    return buffer;
}

char *BufferPrint(char *buffer, const char &ch){
    *buffer = ch;    
    return buffer+1;
}

char *BufferPrint(char *buffer, const int &num){
    return buffer + sprintf(buffer,"%i",num);
}

char *BufferPrint(char *buffer, const long &num){
    return buffer + sprintf(buffer,"%li",num);
}

char *BufferPrint(char *buffer, const long long &num){
    return buffer + sprintf(buffer,"%lli",num);
}

char *BufferPrint(char *buffer, const unsigned int &num){
    return buffer + sprintf(buffer,"%u",num);
}

char *BufferPrint(char *buffer, const unsigned long &num){
    return buffer + sprintf(buffer,"%lu",num);
}

char *BufferPrint(char *buffer, const unsigned long long &num){
    return buffer + sprintf(buffer,"%llu",num);
}

char *BufferPrint(char *buffer, const float &num){
    return buffer + sprintf(buffer,"%f",num);
}

char *BufferPrint(char *buffer, const double &num){
    return buffer + sprintf(buffer,"%lf",num);
}
char *BufferPrint(char *buffer, const void *ptr){
    return buffer + sprintf(buffer,"%p",ptr);
}


void Output::Print(const char *string){
    std::cout << string << "\n";
}

void Output::Printf(const char *fmt){
    std::cout << fmt;
    std::cout.flush();
}



void Output::BufferPrintf(char *buffer, const char *string){
    while(*string!=0){
        *buffer=*string;
        ++buffer;
        ++string;
    }
}

}; // namespace StraitX::