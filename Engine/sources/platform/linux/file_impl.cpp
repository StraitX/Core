#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>
#include "platform/file.hpp"
#include "platform/io.hpp"

namespace StraitX{

constexpr u32 FileCreatePermission = 0664;

Result File::Open(const char *filename, Mode mode){
    assert(m_FD == InvalidFD);
    assert(mode != Mode::Unknown);

    errno = 0;

    m_FD = open(filename, O_CREAT | (int)mode, FileCreatePermission);

    if(m_FD == InvalidFD){
        switch (errno) {
        case EEXIST: return Result::AlreadyExist;
        case EACCES: return Result::PermissionDenied;
        case EISDIR: return Result::IsDirectory;
        }
        return Result::Failure;
    }

    m_Mode = mode;
    return Result::Success;
}

void File::Close(){
    assert(m_FD != InvalidFD);

    close(m_FD);

    m_FD = InvalidFD;
    m_Mode = Mode::Unknown;
}

size_t File::Read(void *buffer, size_t size){
    assert(m_Mode == Mode::Read || m_Mode == Mode::ReadWrite);
    assert(m_FD != InvalidFD);

    return read(m_FD, buffer, size);
}

size_t File::Write(const void *buffer, size_t size){
    assert(m_Mode == Mode::Write || m_Mode == Mode::ReadWrite);
    assert(m_FD != InvalidFD);

    return write(m_FD, buffer, size);
}

size_t File::Seek(SeekPos position, size_t offset){
    assert(m_FD != InvalidFD);

    return lseek(m_FD, offset, position);
}

size_t File::Tell(){
    assert(m_FD != InvalidFD);

    return lseek(m_FD, 0, SEEK_CUR);
}

size_t File::Size(){
    assert(m_FD != InvalidFD);

    struct stat st;
    fstat(m_FD, &st);
    return st.st_size;
}

void File::Shorten(size_t size){
    assert(m_FD != InvalidFD);

    ftruncate(m_FD, Size() - size);
}

Result File::Delete(const char *filename){
    return ResultError(unlink(filename) == -1);
}

bool File::Exist(const char *filename){
    return access(filename, F_OK) == 0;
}

} // namespace StraitX::