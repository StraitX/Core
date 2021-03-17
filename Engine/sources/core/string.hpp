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

}//namespace StraitX

#endif // STRAITX_STRING_HPP