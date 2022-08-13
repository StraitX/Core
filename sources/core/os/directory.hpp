#ifndef STRAITX_DIRECTORY_HPP
#define STRAITX_DIRECTORY_HPP

#include "core/types.hpp"
#include "core/result.hpp"
#include "core/string.hpp"
#include "core/string_view.hpp"
#include "core/noncopyable.hpp"

class DirectoryIterator;

struct DirectoryEntry{
	String Name;
	bool IsFile = false;
	bool IsDirectory = false;
};

class DirectoryIterator {
private:
	void *m_Impl = nullptr;
	DirectoryEntry m_CurrentEntry;
private:
	DirectoryIterator() = default;

	DirectoryIterator(StringView path);

	friend class Directory;
	friend class DirectoryEntry;
public:
	~DirectoryIterator();

	bool operator!=(const DirectoryIterator& other)const;

	DirectoryIterator& operator++();

	const DirectoryEntry &operator*()const;

	const DirectoryEntry *operator->()const;
};

class Directory {
private:
	StringView m_Path;
public:
	Directory(StringView path) :
		m_Path(path)
	{}

	DirectoryIterator begin()const {
		return { m_Path };
	}

	DirectoryIterator end()const {
		return {};
	}

	static bool Exists(StringView path);

	static bool IsDirectory(StringView path);
};

#endif//STRAITX_DIRECTORY_HPP