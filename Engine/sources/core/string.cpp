#include "core/string.hpp"

namespace StraitX{

bool String::Contains(const char *string, const char *internal){
    return Find(string,internal);
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

size_t String::Length(const char *string){
    size_t length = 1;
    while(*string++){
        ++length;
    }
    return length;
}

size_t String::LineLength(const char *string){
    size_t length = 1;
    while(*string != '\n' && *string != '\0'){
        ++string;
        ++length;
    }
    return length;
}


s32 String::Compare(const char *first, const char *second){
    while(*first && (*first == *second)){
        ++first;
        ++second;
    }
    return *first - *second;
}

bool String::Equals(const char *first, const char *second){
    return Compare(first, second) == 0;
}

const char *String::Find(const char *string, const char *internal){
    for(;*string; ++string){
        const char *it = internal;
        const char *out = string;
        for(; *it && *out; ++it, ++out){
            if(*it != *out)
                goto wrong;
        }
        return string;
wrong:
        (void)0;
    }
    return nullptr;
}

char *String::Find(char *string, const char *internal){
    return (char*)Find((const char*)string, internal);
}

const char *String::Find(const char *string, size_t limit, const char *internal){
    for(size_t i = 0;*string && i<limit; ++string){
        size_t j = i;
        const char *it = internal;
        const char *out = string;
        for(; *it && *out && j<limit; ++it, ++out){
            if(*it != *out)
                goto wrong;
        }
        return string;
wrong:
        (void)0;
    }
    return nullptr;
}

char *String::Find(char *string, size_t limit, const char *internal){
    return (char*)Find((const char*)string, limit, internal);
}

}//namespace StraitX::