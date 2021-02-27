#ifndef STRAITX_STRING_HPP
#define STRAITX_STRING_HPP

#include "platform/types.hpp"

namespace StraitX{

class String{
public:
    static bool Contain(const char *string, const char *internal);

    static void ToUpperCase(char *string);

    static void ToLowerCase(char *string);
    // nul character is also a character
    static size_t Length(const char *string);

    static s32 Compare(const char *first, const char *second);

    static bool Equals(const char *first, const char *second);
};

}//namespace StraitX

#endif // STRAITX_STRING_HPP