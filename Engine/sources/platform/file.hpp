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

    //You should close file manually
    sx_inline File(const char *filename, Mode mode){
        (void)Open(filename, mode);
    }

    Result Open(const char *filename, Mode mode);

    sx_inline bool IsOpen(){
        return m_FD != InvalidFD;
    }

    void Close();

    size_t Read(void *buffer, size_t size);

    size_t Write(const void *buffer, size_t size);

    size_t Seek(SeekPos position, size_t offset);

    size_t Tell();

    size_t Size();

    //Result Delete();

    static Result Delete(const char *filename);

    static bool Exist(const char *filename);

};


}; // namespace StraitX:
#endif // STRAITX_FILE_HPP