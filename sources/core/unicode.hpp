#ifndef STRAITX_UNICODE_HPP
#define STRAITX_UNICODE_HPP

#include "core/types.hpp"

#define SX_UTF8(str) u8##str
#define SX_UTF16(str) u##str
#define SX_UTF32(str) U##str

struct Codepoint {
	u32 Value = 0;

	Codepoint() = default;

	Codepoint(const char* string) {
		Decode((const u8*)string);
	}

	Codepoint(const u8* utf8) {
		Decode(utf8);
	}

	Codepoint(const u16* utf16) {
		Decode(utf16);
	}

	Codepoint(const u32* utf32) {
		Decode(utf32);
	}

	size_t Encode(char *utf8)const;

	size_t Encode(u8 *utf8)const;

	size_t Encode(u16 *utf16)const;

	size_t Encode(u32 *utf32)const;

	size_t Decode(const char* utf8);

	size_t Decode(const u8* utf8);

	size_t Decode(const u16* utf16);

	size_t Decode(const u32* utf32);
};

class UnicodeIterator {
private:
	const char* String = nullptr;
	Codepoint CurrentCodepoint;
	u8 DecodedSize = 0;
public:
	UnicodeIterator(const char* string) {
		static_assert(sizeof(char) == sizeof(u8), "types mismatch");
		String = string;
		DecodedSize = CurrentCodepoint.Decode((const u8*)string);
	}

	u32 operator*()const {
		return CurrentCodepoint.Value;
	}

	UnicodeIterator& operator++() {
		String += DecodedSize;
		DecodedSize = CurrentCodepoint.Decode((const u8*)String);
		return *this;
	}

	bool operator!=(const UnicodeIterator& other) {
		return String != other.String;
	}
};

class UnicodeString{
	const char *m_String;
	size_t m_Size;
public:
	UnicodeString(const char *string, size_t size):
		m_String(string),
		m_Size(size)
	{}

	UnicodeIterator begin()const {
		return UnicodeIterator(m_String);
	}

	UnicodeIterator end()const {
		return UnicodeIterator(m_String + m_Size);
	}
};

#endif//STRAITX_UNICODE_HPP