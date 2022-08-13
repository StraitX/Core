#include "platform/windows/wchar.hpp"
#include <Windows.h>

namespace Windows {

std::wstring Utf8ToWstr(const StringView& str) {

	if (!str.Size())
		return {};


	int sizeRequired = MultiByteToWideChar(CP_UTF8, 0, str.Data(), str.Size(), nullptr, 0);

	if (sizeRequired <= 0)
		return {};


	std::wstring retStr(sizeRequired, L'\0');

	int bytesConverted = MultiByteToWideChar(CP_UTF8, 0, str.Data(), str.Size(), &retStr[0], retStr.size());

	if (bytesConverted <= 0)
		std::fprintf(stderr, "Can't convert string to UTF-16");

	return retStr;
}
std::wstring Utf8ToWPath(const StringView& str) {
	std::wstring wpath = Utf8ToWstr(str);
	
	for (wchar_t& ch : wpath) {
		if (ch == L'/')
			ch == L'\\';
	}

	return wpath;
}

String WstrToUtf8(const wchar_t* wstr){
	if (!wstr)
		return {};

	size_t len = wcslen(wstr);

	int sizeRequired = WideCharToMultiByte(CP_UTF8, 0, wstr, len, nullptr, 0, nullptr, nullptr);

    if (sizeRequired <= 0)
		return {};
	
	String res(sizeRequired);

	int bytesConverted = WideCharToMultiByte(CP_UTF8, 0, wstr, len, res.Data(), res.Size(), nullptr, nullptr);

	if(bytesConverted <= 0)
		std::fprintf(stderr, "Can't convert string to UTF-8");

	return res;
}

String WPathToUtf8(const wchar_t* str){
	String path = WstrToUtf8(str);
	
	for (int i = 0; i < path.Size(); i++) {
		if (path.Data()[i] == '\\')
			path.Data()[i] = '/';
	}

	return path;
}

}//namespace Windows::
