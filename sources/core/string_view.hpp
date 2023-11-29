#ifndef STRAITX_STRING_VIEW_HPP
#define STRAITX_STRING_VIEW_HPP

#include <string>
#include "core/unicode.hpp"
#include "core/move.hpp"
#include "core/printer.hpp"
#include "core/mixins.hpp"

class StringView: public StringMixin<StringView>{
protected:
	const char *m_String = nullptr;
	size_t m_CodeunitsCount = 0;
public:
	constexpr StringView() = default;
	
	StringView(const std::string &string):
		StringView(string.data(), string.size())
	{}

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

	constexpr size_t CodeunitsCount()const{
		return m_CodeunitsCount;
	}

	char operator[](size_t index)const{
		SX_ASSERT(index < Size());
		return Data()[index];
	}

	const char* begin()const {
		return Data();
	}

	const char* end()const {
		return Data() + Size();
	}
};

inline bool operator==(const StringView &left, const StringView &right) {
	if(left.Size() != right.Size())
		return false;
	
	for(size_t i = 0; i<left.Size(); i++)
		if(left[i] != right[i])
			return false;

	return true;
}

inline bool operator!=(const StringView &left, const StringView &right) {
	return !(left == right);
}

template<>
struct Printer<StringView> {
	static void Print(const StringView& value, StringWriter &writer) {
		writer.Write(value.Data(), value.CodeunitsCount());
	}
};

namespace std {
	template<typename T>
	struct hash;

	template<>
	struct hash<StringView>{
		size_t operator()(StringView view)const {
			unsigned int hash = 5381;

			for (size_t i = 0; i < view.Size(); ++i) {
				hash = ((hash << 5) + hash) + static_cast<unsigned char>(view.Data()[i]);
			}

			return hash;
		}
	};
}

#endif//STRAITX_STRING_VIEW_HPP