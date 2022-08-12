#include <windows.h>
#include <assert.h>
#include "core/os/file.hpp"

static_assert(sizeof(HANDLE) <= sizeof(u64),"Win32 Handle can't fit into File::m_FD");

Result File::Open(StringView filename, Mode mode, bool create) {
	assert(m_FD == InvalidFD);

	OFSTRUCT open_file_struct = {};
	m_FD = OpenFile(filename.Data(), &open_file_struct, (unsigned int)mode | (create ? (!Exists(filename) ? OF_CREATE : 0) : 0));

	if (m_FD == HFILE_ERROR) { 
		m_FD = InvalidFD;
		return Result::Failure; 
	}

	m_Mode = mode;

	return Result::Success;
}

void File::Close() {
	(void)CloseHandle(reinterpret_cast<HANDLE>(m_FD));
	m_FD = InvalidFD;
}

size_t File::Read(void* buffer, size_t size) {
	assert(m_FD != InvalidFD);
	assert(m_Mode == Mode::Read || m_Mode == Mode::ReadWrite);

	DWORD read = 0;

	if (!ReadFile(reinterpret_cast<HANDLE>(m_FD), buffer, size, &read, nullptr))return 0;

	return read;
}

size_t File::Write(const void* buffer, size_t size) {
	assert(m_FD != InvalidFD);
	assert(m_Mode == Mode::Write || m_Mode == Mode::ReadWrite);

	DWORD write = 0;

	if (!WriteFile(reinterpret_cast<HANDLE>(m_FD), buffer, size, &write, nullptr)) return 0;

	return write;
}

s64 File::Seek(SeekPos position, s64 offset) {
	union {
		s64 Offset;
		struct {
			long Low;
			long High;
		};
	}off;
	off.Offset = offset;
	return SetFilePointer(reinterpret_cast<HANDLE>(m_FD), off.Low, &off.High, (DWORD)position);
}

s64 File::Tell() {
	return Seek(SeekPos::Current, 0);
}

u64 File::Size() {
	union {
		u64 U64;
		struct {
			unsigned long Low;
			unsigned long High;
		};
	}size = {};
	size.Low = GetFileSize(reinterpret_cast<HANDLE>(m_FD), &size.High);
	return size.U64;
}

Result File::Delete(StringView filename) {
	return ResultError(!DeleteFile(filename.Data()));
}

bool File::Exists(StringView filename) {
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile(filename.Data(), &FindFileData);

	if(handle != INVALID_HANDLE_VALUE) {
		FindClose(handle);
		return true;
	}
	return false;
}