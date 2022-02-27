#ifndef STRAITX_STRING_VIEW_HPP
#define STRAITX_STRING_VIEW_HPP

#include "core/types.hpp"
#include "core/unicode.hpp"
#include "core/span.hpp"

class StringView {
protected:
	const char *m_String = nullptr;
	size_t m_CodeunitsCount = 0;
public:
	StringView(const char *string, size_t codeunits_count):
		m_String(string),
		m_CodeunitsCount(codeunits_count)
	{}

	constexpr StringView(const char *string):
		m_String(string),
		m_CodeunitsCount(StaticCodeunitsCount(string))
	{}

	StringView(StringView &&) = delete;

	StringView(const StringView &) = default;

	StringView &operator=(StringView &&) = delete;

	StringView &operator=(const StringView &) = default;

	constexpr StringView& operator=(const char* string){
		m_String = string;
		m_CodeunitsCount = StaticCodeunitsCount(string);
		return *this;
	}

	const char* Data()const{
		return m_String;
	}

	size_t Size()const{
		return CodeunitsCount();
	}

	size_t CodeunitsCount()const{
		return m_CodeunitsCount;
	}

	size_t CodepointsCount()const{
		size_t counter = 0;
		for(u32 ch: *this)
			counter++;
		return counter;
	}

	operator ConstSpan<char>()const{
		return {Data(), Size()};
	}

	UnicodeIterator begin()const{
		return {Data()};
	}

	UnicodeIterator end()const{
		return {Data() + Size()};
	}
protected:
	static constexpr size_t StaticCodeunitsCount(const char* string) {
		if(!string)return 0;

		size_t counter = 0;
		while (*string++) 
			counter++;
		return counter;
	}
};

#endif//STRAITX_STRING_VIEW_HPP