#include "core/printer.hpp"
#include "core/types.hpp"
#include <cstdio>

template<>
struct Printer<const char *>{
	static void Print(const char * const &string, void (*writer)(char, void*), void *writer_data);
};

void Printer<const char *>::Print(const char * const &string, void (*writer)(char, void*), void *writer_data){
	const char *str = string;
	while(*str)
		writer(*str++, writer_data);
}

template<>
struct Printer<const void *>{
	static void Print(const void *const &pointer, void (*writer)(char, void*), void *writer_data);
};

void Printer<const void *>::Print(const void *const &pointer, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%p%c", pointer, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<void *>{
	static void Print(void *const &pointer, void (*writer)(char, void*), void *writer_data);
};

void Printer<void *>::Print(void *const &pointer, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%p%c", pointer, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<char *>{
	static void Print(char *const &string, void (*writer)(char, void*), void *writer_data);
};

void Printer<char *>::Print(char *const &string, void (*writer)(char, void*), void *writer_data){
	Printer<const char *>().Print((const char *)string, writer, writer_data);
}

template<>
struct Printer<char>{
	static void Print(const char &ch, void (*writer)(char, void*), void *writer_data);
};

void Printer<char>::Print(const char &ch, void (*writer)(char, void*), void *writer_data){
	writer(ch, writer_data);
}

template<>
struct Printer<unsigned char>{
	static void Print(const unsigned char &ch, void (*writer)(char, void*), void *writer_data);
};

void Printer<unsigned char>::Print(const unsigned char &ch, void (*writer)(char, void*), void *writer_data){
	writer(ch, writer_data);
}

template<>
struct Printer<short>{
	static void Print(const short &value, void (*writer)(char, void*), void *writer_data);
};

void Printer<short>::Print(const short &value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%i%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<unsigned short>{
	static void Print(const unsigned short &value, void (*writer)(char, void*), void *writer_data);
};

void Printer<unsigned short>::Print(const unsigned short &value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%u%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<int>{
	static void Print(const int &value, void (*writer)(char, void*), void *writer_data);
};

void Printer<int>::Print(const int &value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%i%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<unsigned int>{
	static void Print(const unsigned int &value, void (*writer)(char, void*), void *writer_data);
};

void Printer<unsigned int>::Print(const unsigned int &value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%u%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<long>{
	static void Print(const long &value, void (*writer)(char, void*), void *writer_data);
};

void Printer<long>::Print(const long &value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%li%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<unsigned long>{
	static void Print(const unsigned long &value, void (*writer)(char, void*), void *writer_data);
};

void Printer<unsigned long>::Print(const unsigned long &value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%lu%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<long long>{
	static void Print(const long long &value, void (*writer)(char, void*), void *writer_data);
};
void Printer<long long>::Print(const long long &value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%lli%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<unsigned long long>{
	static void Print(const unsigned long long &value, void (*writer)(char, void*), void *writer_data);
};

void Printer<unsigned long long>::Print(const unsigned long long &value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%llu%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<float>{
	static void Print(const float& value, void (*writer)(char, void*), void *writer_data);
};
void Printer<float>::Print(const float& value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%g%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<double>{
	static void Print(const double& value, void (*writer)(char, void*), void *writer_data);
};
void Printer<double>::Print(const double& value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%g%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}
template<>
struct Printer<long double>{
	static void Print(const long double& value, void (*writer)(char, void*), void *writer_data);
};

void Printer<long double>::Print(const long double& value, void (*writer)(char, void*), void *writer_data){
	char buffer[30];

	sprintf(buffer, "%Lg%c", value, '\0');

	for(int i = 0; buffer[i]; i++)
		writer(buffer[i], writer_data);
}

template<>
struct Printer<Version>{
	static void Print(const Version& value, void (*writer)(char, void*), void *writer_data);
};
void Printer<Version>::Print(const Version& value, void (*writer)(char, void*), void *writer_data){
	Printer<u16> ::Print(value.Major, writer, writer_data);
	Printer<char>::Print('.', writer, writer_data);
	Printer<u16> ::Print(value.Minor, writer, writer_data);
	Printer<char>::Print('.', writer, writer_data);
	Printer<u32> ::Print(value.Patch, writer, writer_data);
}
