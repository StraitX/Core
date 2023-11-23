#include "core/printer.hpp"
#include "core/types.hpp"
#include "core/string_view.hpp"
#include <cstdio>

template<>
struct Printer<const char *>{
	static void Print(const char * const &string, StringWriter &writer);
};

void Printer<const char *>::Print(const char * const &string, StringWriter &writer){
	if(!string)return;
	
	Printer<StringView>::Print(string, writer);
}

template<>
struct Printer<const void *>{
	static void Print(const void *const &pointer, StringWriter &writer);
};

void Printer<const void *>::Print(const void *const &pointer, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%p%c", pointer, '\0');
	
	Printer<const char*>::Print(buffer, writer);
}

template<>
struct Printer<void *>{
	static void Print(void *const &pointer, StringWriter &writer);
};

void Printer<void *>::Print(void *const &pointer, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%p%c", pointer, '\0');

	Printer<const char*>::Print(buffer, writer);
}

template<>
struct Printer<char *>{
	static void Print(char *const &string, StringWriter &writer);
};

void Printer<char *>::Print(char *const &string, StringWriter &writer){
	Printer<const char *>().Print((const char *)string, writer);
}

template<>
struct Printer<bool> {
	static void Print(const bool& b, StringWriter& writer);
};

void Printer<bool>::Print(const bool& b, StringWriter& writer){
	const char *string = b ? "true" : "false";
	Printer<const char *>::Print(string, writer);
}

template<>
struct Printer<char>{
	static void Print(const char &ch, StringWriter &writer);
};

void Printer<char>::Print(const char &ch, StringWriter &writer){
	writer.Write(ch);
}

template<>
struct Printer<unsigned char>{
	static void Print(const unsigned char &ch, StringWriter &writer);
};

void Printer<unsigned char>::Print(const unsigned char &ch, StringWriter &writer){
	writer.Write(ch);
}

template<>
struct Printer<short>{
	static void Print(const short &value, StringWriter &writer);
};

void Printer<short>::Print(const short &value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%i%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}

template<>
struct Printer<signed char>{
	static void Print(const signed char &ch, StringWriter &writer);
};

void Printer<signed char>::Print(const signed char &ch, StringWriter &writer){
	Printer<short>::Print(ch, writer);
}

template<>
struct Printer<unsigned short>{
	static void Print(const unsigned short &value, StringWriter &writer);
};

void Printer<unsigned short>::Print(const unsigned short &value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%u%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}

template<>
struct Printer<int>{
	static void Print(const int &value, StringWriter &writer);
};

void Printer<int>::Print(const int &value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%i%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}

template<>
struct Printer<unsigned int>{
	static void Print(const unsigned int &value, StringWriter &writer);
};

void Printer<unsigned int>::Print(const unsigned int &value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%u%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}

template<>
struct Printer<long>{
	static void Print(const long &value, StringWriter &writer);
};

void Printer<long>::Print(const long &value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%li%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}

template<>
struct Printer<unsigned long>{
	static void Print(const unsigned long &value, StringWriter &writer);
};

void Printer<unsigned long>::Print(const unsigned long &value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%lu%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}

template<>
struct Printer<long long>{
	static void Print(const long long &value, StringWriter &writer);
};
void Printer<long long>::Print(const long long &value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%lli%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}

template<>
struct Printer<unsigned long long>{
	static void Print(const unsigned long long &value, StringWriter &writer);
};

void Printer<unsigned long long>::Print(const unsigned long long &value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%llu%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}

template<>
struct Printer<float>{
	static void Print(const float& value, StringWriter &writer);
};
void Printer<float>::Print(const float& value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%g%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}

template<>
struct Printer<double>{
	static void Print(const double& value, StringWriter &writer);
};
void Printer<double>::Print(const double& value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%g%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}
template<>
struct Printer<long double>{
	static void Print(const long double& value, StringWriter &writer);
};

void Printer<long double>::Print(const long double& value, StringWriter &writer){
	char buffer[30];

	sprintf(buffer, "%Lg%c", value, '\0');

	Printer<const char *>::Print(buffer, writer);
}

template<>
struct Printer<Version>{
	static void Print(const Version& value, StringWriter &writer);
};
void Printer<Version>::Print(const Version& value, StringWriter &writer){
	Printer<u16> ::Print(value.Major, writer);
	Printer<char>::Print('.', writer);
	Printer<u16> ::Print(value.Minor, writer);
	Printer<char>::Print('.', writer);
	Printer<u32> ::Print(value.Patch, writer);
}
