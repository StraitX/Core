#include "core/os/directory.hpp"
#include "platform/windows/wchar.hpp"
#include <Windows.h>

bool IsSpecialDirectory(const wchar_t* path) {
	SX_CORE_ASSERT(path[0], "Something goes wrong, empty path");
	return (path[0] == '.' && path[1] == '\0') || (path[0] == '.' && path[1] == '.' && path[2] == '\0');
}

struct DirectoryIteratorImpl {
	HANDLE CurrentFile = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATAW CurrentFileMetadata = {};

	DirectoryIteratorImpl(StringView path, DirectoryEntry &entry) {
		std::wstring wpath = Windows::Utf8ToWPath(path);
		if(wpath.back() != L'/')
			wpath.push_back(L'/');
		wpath.push_back(L'*');
		wpath.push_back(L'*');

		CurrentFile = FindFirstFileW(wpath.c_str(), &CurrentFileMetadata);
		while (IsSpecialDirectory(CurrentFileMetadata.cFileName)) {
			if (!FindNextFileW(CurrentFile, &CurrentFileMetadata)) {
				Invalidate();
				break;
			}
		}

		if (!IsValid())return;
		
		entry = DirectoryEntry{
			Name(),
			IsFile(),
			IsDirectory()
		};
	}

	~DirectoryIteratorImpl() {
		Invalidate();
	}

	void MoveNext(DirectoryEntry &entry) {
		entry = DirectoryEntry();

		do {
			if (!FindNextFileW(CurrentFile, &CurrentFileMetadata)) {
				Invalidate();
				break;
			}
		} while (IsSpecialDirectory(CurrentFileMetadata.cFileName));

		if (!IsValid())return;
		
		entry = DirectoryEntry{
			Name(),
			IsFile(),
			IsDirectory()
		};
	}

	bool IsValid()const {
		return CurrentFile != INVALID_HANDLE_VALUE;
	}

	void Invalidate() {
		if (IsValid())
			FindClose(CurrentFile);
		CurrentFile = INVALID_HANDLE_VALUE;
	}

	bool IsFile()const {
		return IsValid()
			&& !(CurrentFileMetadata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool IsDirectory()const {
		return IsValid()
			&& CurrentFileMetadata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
	}

	String Name()const {
		return Windows::WstrToUtf8(CurrentFileMetadata.cFileName);
	}
};

static auto ToDirItImpl(void* ptr) {
	return reinterpret_cast<DirectoryIteratorImpl*>(ptr);
}

DirectoryIterator::DirectoryIterator(StringView path){
	if (!Directory::Exists(path))return;

	m_Impl = new DirectoryIteratorImpl(path, m_CurrentEntry);
}

DirectoryIterator::~DirectoryIterator() {
	delete ToDirItImpl(m_Impl);
}

bool DirectoryIterator::operator!=(const DirectoryIterator& other)const {
	return m_Impl != other.m_Impl;
}

DirectoryIterator& DirectoryIterator::operator++() {
	auto impl = ToDirItImpl(m_Impl);
	impl->MoveNext(m_CurrentEntry);
	if (!impl->IsValid()) {
		delete impl;
		m_Impl = nullptr;
	}
	return *this;
}

const DirectoryEntry &DirectoryIterator::operator*()const {
	return m_CurrentEntry;
}

const DirectoryEntry *DirectoryIterator::operator->()const {
	return &m_CurrentEntry;
}

bool Directory::Exists(StringView path) {
	return IsDirectory(path);
}

bool Directory::IsDirectory(StringView path) {
	DWORD ftyp = GetFileAttributesW(Windows::Utf8ToWPath(path).c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;

	return (ftyp & FILE_ATTRIBUTE_DIRECTORY);	
}

String Directory::Current() {

	int len = GetCurrentDirectoryW(0, nullptr);

	std::wstring path(len, '\0');

	GetCurrentDirectoryW(len, &path[0]);

	return Windows::WPathToUtf8(path.c_str());
}

bool Directory::Change(StringView path) {
	return SetCurrentDirectoryW(Windows::Utf8ToWPath(path).c_str()) != 0;
}

Result Directory::Delete(StringView path) {
	auto wpath = Windows::Utf8ToWPath(path) + L'\0';
    SHFILEOPSTRUCTW file_op = {
        NULL,
        FO_DELETE,
        wpath.c_str(),
        L"",
        FOF_NOCONFIRMATION |
        FOF_NOERRORUI |
        FOF_SILENT,
        false,
        0,
        L"" };
    return Result(!SHFileOperationW(&file_op));
}