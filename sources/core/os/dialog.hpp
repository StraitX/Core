#ifndef STRAITX_DIALOG_HPP
#define STRAITX_DIALOG_HPP

#include <core/string.hpp>

struct Dialog {
	static String SelectFolder(StringView title);

	static String SelectFile(StringView title, Span<StringView> extensions);
};

#endif//STRAITX_DIALOG_HPP