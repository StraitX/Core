#ifndef STRAITX_FILE_HPP
#define STRAITX_FILE_HPP


#include "platform/types.hpp"
#include "platform/result.hpp"
#include "platform/noncopyable.hpp"

namespace StraitX{
//TODO take care about size_t and 64 32 bit OS
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

    ~File();

    File(const char *filename, Mode mode, bool create = true);

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

sx_inline File::~File(){
    if(IsOpen())
        Close();
}

sx_inline File::File(const char *filename, Mode mode, bool create){
    (void)Open(filename, mode, create);
}

sx_inline bool File::IsOpen(){
    return m_FD != InvalidFD;
}

}; // namespace StraitX:
#endif // STRAITX_FILE_HPP