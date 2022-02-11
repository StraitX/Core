#ifndef STRAITX_STRING_VIEW_HPP
#define STRAITX_STRING_VIEW_HPP

#include "core/types.hpp"
#include "core/string.hpp"
#include "core/unicode.hpp"

class StringView {
private:
	const char *m_String = nullptr;
	size_t m_Size = 0;
public:
	StringView(const char *string, size_t size):
		m_String(string),
		m_Size(size)
	{}

	constexpr StringView(const char *string):
		m_String(string),
		m_Size(BytesSize(string))
	{}

	StringView(const StringView &) = default;

	StringView &operator=(const StringView &) = default;

	constexpr StringView& operator=(const char* string) {
		*this = StringView(string);
		return *this;
	}

	const char* Data()const {
		return m_String;
	}

	size_t Size()const {
		return m_Size;
	}

	size_t UnicodeSize()const {
		size_t counter = 0;
		for(u32 ch: *this)
			counter++;
		return counter;
	}

	UnicodeIterator begin()const{
		return {Data()};
	}

	UnicodeIterator end()const{
		return {Data()  + Size()};
	}
private:
	static constexpr size_t BytesSize(const char* string) {
		size_t counter = 0;
		while (*string++) 
			counter++;
		return counter;
	}
};

#endif//STRAITX_STRING_VIEW_HPP