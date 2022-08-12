#ifndef STRAITX_WINDOWS_WCHAR_HPP
#define STRAITX_WINDOWS_WCHAR_HPP

#include "core/string.hpp"
#include <string>

namespace Windows {

std::wstring Utf8ToWstr(const StringView& str);

std::wstring Utf8ToWPath(const StringView& str);

}//namespace Windows::

#endif//STRAITX_WINDOWS_WCHAR_HPP