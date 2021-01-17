#include "core/string.hpp"

namespace StraitX{

bool String::Contain(const char *string, const char *internal){
    bool contain = false;
    for(;*string; ++string){
        for(;*internal;++internal){
            if(*string != *internal)
                goto wrong; 
        }
        contain = true;
wrong:    
        ((void)0);
    }
    return contain;
}

void String::ToUpperCase(char *string){
    for(;*string; ++string){
        if(*string <= 'z' && *string >= 'a')
            *string += 'A'-'a';
    }
}

void String::ToLowerCase(char *string){
    for(;*string; ++string){
        if(*string <= 'Z' && *string >= 'A')
            *string += 'a'-'A';
    }
}

size_t String::Lenght(const char *string){
    size_t lenght = 1;
    while(*string++){
        ++lenght;
    }
    return lenght;
}

}//namespace StraitX::