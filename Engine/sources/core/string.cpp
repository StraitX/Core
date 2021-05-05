#include "core/string.hpp"

namespace StraitX{

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
    size_t length = 0;
    while(*string++){
        ++length;
    }
    return length;
}

size_t String::LineLength(const char *string){
    size_t length = 0;
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

const char *String::Find(const char *string, size_t limit, const char *internal){
    for(size_t i = 0;*string && i<limit; ++string, ++i){
        size_t j = i;
        const char *it = internal;
        const char *out = string;
        for(; *it && *out && j<limit; ++it, ++out, ++j){
            if(*it != *out)
                goto wrong;
        }
        return string;
wrong:
        (void)0;
    }
    return nullptr;
}

const char *String::FindLast(const char *string, const char *internal){
    const char *result = nullptr;
    for(;*string;++string){
        auto in = string;
        auto out = internal;
        for(; *in && *out; ++in, ++out)
            if(*in != *out)goto wrong;
        result = string;
wrong:
        (void)0;
    }
    return result;
}

const char *String::IgnoreUntil(const char *string, char ch){
    while(*string && *string != ch)string++;
    return string;
}

const char *String::Ignore(const char *string, char ch){
    while(*string && *string == ch)string++;
    return string;
}

}//namespace StraitX::