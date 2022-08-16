#ifndef STRAITX_STRING_PRINT_HPP
#define STRAITX_STRING_PRINT_HPP

#include "core/print.hpp"
#include "core/string.hpp"

template <typename ...ArgsType>
String StringPrint(const char *fmt, const ArgsType&...args) {
	class ToStringWriter: public StringWriter{
	private:
		String& m_String;
	public:
		ToStringWriter(String &string):
			m_String(string)
		{}

		void Write(const char* string, size_t size)override{
			m_String.Append({ string, size });
		}
	};
	
	String result;
	ToStringWriter writer(result);
	WriterPrint(writer, fmt, args...);
	return result;
}

template <typename ...ArgsType>
String StringPrintln(const char* fmt, const ArgsType&...args) {
	String result = StringPrint(fmt, args...);
	result.Append("\n");
	return result;
}

#endif//STRAITX_STRING_PRINT_HPP