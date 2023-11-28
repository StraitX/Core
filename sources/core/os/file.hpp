#ifndef STRAITX_FILE_HPP
#define STRAITX_FILE_HPP


#include "core/types.hpp"
#include "core/result.hpp"
#include "core/noncopyable.hpp"
#include "core/string_view.hpp"
#include "core/string.hpp"
#include "core/optional.hpp"

class File: public NonCopyable{
public:
    enum class Mode{
        Read        = 0,
        Write       = 1,
        ReadWrite   = 2
    };

    enum SeekPos{
        Begin   = 0,
        Current = 1,
        End     = 2
    };
    static constexpr u64 InvalidFD = -1;
private:
    u64 m_FD = InvalidFD;
    Mode m_Mode = Mode::Read;
public:
    File() = default;

    File(File &&other)noexcept;

    File(StringView filename, Mode mode, bool create = true);

    ~File();

    File &operator=(File &&other)noexcept;

    Result Open(StringView filename, Mode mode, bool create = true);

    bool IsOpen()const;

    void Close();

    size_t Read(void *buffer, size_t size);

    size_t Write(const void *buffer, size_t size);

    s64 Seek(SeekPos position, s64 offset);

    s64 Tell();

    u64 Size();

    static Result Delete(StringView filename);

    static Result Delete(const char *filename);

    static bool Exists(StringView filename);

    static bool Exists(const char *filename);
    
    static bool IsFile(StringView filename);

    static bool IsFile(const char *filename);

    static Optional<String> ReadEntire(StringView filename);

    static bool WriteEntire(StringView filename, StringView content);

};

SX_INLINE File::File(File &&other)noexcept{
    *this = (File&&)other;
}

SX_INLINE File::~File(){
    if(IsOpen())
        Close();
}

SX_INLINE File &File::operator=(File &&other)noexcept{
    m_FD = other.m_FD;
    m_Mode = other.m_Mode;
    other.m_FD = InvalidFD;
    other.m_Mode = Mode::Read;
    return *this;
}

SX_INLINE File::File(StringView filename, Mode mode, bool create){
    (void)Open(filename, mode, create);
}

SX_INLINE bool File::IsOpen()const{
    return m_FD != InvalidFD;
}


SX_INLINE Result File::Delete(const char* filename) {
    return File::Delete(StringView(filename));
}


SX_INLINE bool File::Exists(const char* filename) {
    return File::Exists(StringView(filename));
}

SX_INLINE bool File::IsFile(const char* filename) {
    return File::IsFile(StringView(filename));
}

#endif // STRAITX_FILE_HPP