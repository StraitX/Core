#ifndef STRAITX_STRING_VIEW_HPP
#define STRAITX_STRING_VIEW_HPP

#include "core/types.hpp"
#include "core/unicode.hpp"
#include "core/span.hpp"
#include "core/move.hpp"
#include "core/printer.hpp"

class StringView {
protected:
	const char *m_String = nullptr;
	size_t m_CodeunitsCount = 0;
public:
	constexpr StringView() = default;

	constexpr StringView(const char *string, size_t codeunits_count):
		m_String(string),
		m_CodeunitsCount(codeunits_count)
	{}

	constexpr StringView(const char *string):
		m_String(string),
		m_CodeunitsCount(StaticCodeunitsCount(string))
	{}

	constexpr StringView(StringView &&other)noexcept{
		*this = Move(other);
	}

	constexpr StringView(const StringView &) = default;

	constexpr StringView& operator=(StringView &&other)noexcept{
		m_String = other.m_String;
		m_CodeunitsCount = other.m_CodeunitsCount;
		other.m_String = nullptr;
		other.m_CodeunitsCount = 0;
		return *this;
	}

	constexpr StringView &operator=(const StringView &) = default;

	constexpr StringView& operator=(const char* string){
		m_String = string;
		m_CodeunitsCount = StaticCodeunitsCount(string);
		return *this;
	}

	constexpr const char* Data()const{
		return m_String;
	}

	constexpr size_t Size()const{
		return CodeunitsCount();
	}

	constexpr size_t CodeunitsCount()const{
		return m_CodeunitsCount;
	}

	size_t CodepointsCount()const{
		size_t counter = 0;
		for(u32 ch: *this)
			counter++;
		return counter;
	}

	constexpr operator ConstSpan<char>()const{
		return {Data(), Size()};
	}

	UnicodeIterator begin()const{
		return {Data()};
	}

	UnicodeIterator end()const{
		return {Data() + Size()};
	}

	static constexpr size_t StaticCodeunitsCount(const char* string) {
		if(!string)return 0;

		size_t counter = 0;
		while (*string++) 
			counter++;
		return counter;
	}
};

template<>
struct Printer<StringView> {
	static void Print(const StringView& value, StringWriter &writer) {
		writer.Write(value.Data(), value.CodeunitsCount());
	}
};

#endif//STRAITX_STRING_VIEW_HPP