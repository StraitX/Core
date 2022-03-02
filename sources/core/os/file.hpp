#ifndef STRAITX_FILE_HPP
#define STRAITX_FILE_HPP


#include "core/types.hpp"
#include "core/result.hpp"
#include "core/noncopyable.hpp"

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

    File(const char *filename, Mode mode, bool create = true);

    ~File();

    File &operator=(File &&other)noexcept;

    Result Open(const char *filename, Mode mode, bool create = true);

    bool IsOpen();

    void Close();

    size_t Read(void *buffer, size_t size);

    size_t Write(const void *buffer, size_t size);

    s64 Seek(SeekPos position, s64 offset);

    s64 Tell();

    u64 Size();

    static Result Delete(const char *filename);

    static bool Exist(const char *filename);

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

SX_INLINE File::File(const char *filename, Mode mode, bool create){
    (void)Open(filename, mode, create);
}

SX_INLINE bool File::IsOpen(){
    return m_FD != InvalidFD;
}

#endif // STRAITX_FILE_HPP