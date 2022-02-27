#ifndef STRAITX_STRING_HPP
#define STRAITX_STRING_HPP

#include "core/types.hpp"
#include "core/move.hpp"
#include "core/noncopyable.hpp"
#include "core/string_view.hpp"
#include "core/os/memory.hpp"

class String: public StringView, public NonCopyable{
public:
    String():
        StringView(nullptr, 0)
    {}

    String(const char *string):
        String(string, StaticCodeunitsCount(string))
    {}

    String(const char *string, size_t length):
        StringView(
            (char*)Memory::Alloc((length + 1) * sizeof(char)),
            length
        )
    {
        Memory::Copy(string, Data(), length);
        Data()[length] = 0;
    }

    String(String&& other)noexcept:
        StringView(nullptr, 0)
    {
        *this = Move(other);
    }

    ~String() {
        Clear();
    }

    String& operator=(String&& other)noexcept {
        Clear();
        m_String = other.m_String;
        m_CodeunitsCount = other.m_CodeunitsCount;
        other.m_String = nullptr;
        other.m_CodeunitsCount = 0;
        return *this;
    }

    void Clear() {
        Memory::Free(Data());
        m_String = nullptr;
        m_CodeunitsCount = 0;
    }
    
    char* Data(){
        return const_cast<char*>(m_String);
    }
    
    //XXX: Do something about this
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

    static const char *FindLast(const char *string, const char *internal);

    static char *FindLast(char *string, const char *internal);

    static const char *IgnoreUntil(const char *string, char ch);

    static char *IgnoreUntil(char *string, char ch);

    static const char *Ignore(const char *string, char ch);

    static char *Ignore(char *string, char ch);
};

template<>
struct Printer<String> {
	static void Print(const String& value, void (*writer)(char, void*), void* writer_data) {
		Printer<StringView>::Print(value, writer, writer_data);
	}
};

SX_INLINE bool String::Contains(const char *string, const char *internal){
    return Find(string,internal);
}

SX_INLINE bool String::Contains(const char *string, size_t limit, const char *internal){
    return Find(string, limit, internal);
}

SX_INLINE bool String::Equals(const char *first, const char *second){
    return Compare(first, second) == 0;
}

SX_INLINE char *String::Find(char *string, const char *internal){
    return (char*)Find((const char*)string, internal);
}

SX_INLINE char *String::Find(char *string, size_t limit, const char *internal){
    return (char*)Find((const char*)string, limit, internal);
}

SX_INLINE char *String::FindLast(char *string, const char *internal){
    return (char*)FindLast((const char *)string, internal);
}

SX_INLINE char *String::IgnoreUntil(char *string, char ch){
    return (char*)IgnoreUntil((const char*)string, ch);
}

SX_INLINE char *String::Ignore(char *string, char ch){
    return (char *)Ignore((const char *)string, ch);
}

#endif // STRAITX_STRING_HPP