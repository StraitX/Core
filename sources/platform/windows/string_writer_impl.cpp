#include "core/string_writer.hpp"
#include "core/os/memory.hpp"
#include <Windows.h>
#include <cstdio>
#include <string>

std::wstring Utf8ToWstr(const char *buffer, size_t size){

	if (!size)
		return {};


	int sizeRequired = MultiByteToWideChar(CP_UTF8, 0, buffer, size, nullptr, 0);

    if (sizeRequired <= 0)
		return {};


	std::wstring retStr(sizeRequired, L'\0');

	int bytesConverted = MultiByteToWideChar(CP_UTF8, 0, buffer, size, &retStr[0], retStr.size());

	if (bytesConverted <= 0)
		return {};

	return retStr;
}


struct OutStringWriter: StringWriter{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	void Write(const char *buffer, size_t size)override{
		auto utf16 = Utf8ToWstr(buffer, size);
		WriteConsoleW(hStdOut, utf16.data(), utf16.size(), nullptr, nullptr);
	}
};

struct ErrorStringWriter: StringWriter{
	HANDLE hStdError = GetStdHandle(STD_ERROR_HANDLE);

	void Write(const char *buffer, size_t size)override{
		auto utf16 = Utf8ToWstr(buffer, size);
		WriteConsoleW(hStdError, utf16.data(), utf16.size(), nullptr, nullptr);
	}
};

static OutStringWriter WindowsStraitXOut;
static ErrorStringWriter WindowsStraitXError;

StringWriter* StraitXOut = &WindowsStraitXOut;
StringWriter* StraitXError = &WindowsStraitXError;