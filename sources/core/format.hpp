#pragma once

#include "core/print.hpp"
#include "core/string.hpp"

template <typename ...ArgsType>
String Format(const char *fmt, const ArgsType&...args) {
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

