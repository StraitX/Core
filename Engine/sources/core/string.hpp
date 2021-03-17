#ifndef STRAITX_STRING_HPP
#define STRAITX_STRING_HPP

#include "platform/types.hpp"

namespace StraitX{

class String{
public:
    static bool Contains(const char *string, const char *internal);

    static bool Contains(const char *string, size_t limit, const char *internal);

    static void ToUpperCase(char *string);

    static void ToLowerCase(char *string);
    // nul character is also a character
    static size_t Length(const char *string);
    //nil or \n characters are included
    static size_t LineLength(const char *string);

    static s32 Compare(const char *first, const char *second);

    static bool Equals(const char *first, const char *second);

    static const char *Find(const char *string, const char *internal);

    static char *Find(char *string, const char *internal);

    static const char *Find(const char *string, size_t limit, const char *internal);

    static char *Find(char *string, size_t limit, const char *internal);

    static const char *IgnoreUntil(const char *string, char ch);

    static char *IgnoreUntil(char *string, char ch);

    static const char *Ignore(const char *string, char ch);

    static char *Ignore(char *string, char ch);
};

sx_inline bool String::Contains(const char *string, const char *internal){
    return Find(string,internal);
}

sx_inline bool String::Contains(const char *string, size_t limit, const char *internal){
    return Find(string, limit, internal);
}

sx_inline bool String::Equals(const char *first, const char *second){
    return Compare(first, second) == 0;
}

sx_inline char *String::Find(char *string, const char *internal){
    return (char*)Find((const char*)string, internal);
}

sx_inline char *String::Find(char *string, size_t limit, const char *internal){
    return (char*)Find((const char*)string, limit, internal);
}

sx_inline char *String::IgnoreUntil(char *string, char ch){
    return (char*)IgnoreUntil((const char*)string, ch);
}

sx_inline char *String::Ignore(char *string, char ch){
    return (char *)Ignore((const char *)string, ch);
}

}//namespace StraitX

#endif // STRAITX_STRING_HPP