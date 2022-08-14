#include "core/os/file.hpp"

Optional<String> File::ReadEntire(StringView filename) {
	File file;
	if (!file.Open(filename, File::Mode::Read, false))
		return {};
	
	String buffer(file.Size());
	if (file.Read(buffer.Data(), buffer.Size()) != buffer.Size())
		return {};

	return { Move(buffer) };
}

bool File::WriteEntire(StringView filename, StringView content) {
	File file;
	if (!file.Open(filename, File::Mode::Write, true))
		return false;

	return file.Write(content.Data(), content.Size()) == content.Size();
}