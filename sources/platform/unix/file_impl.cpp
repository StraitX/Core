#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>
#include "core/os/file.hpp"
#include "core/env/os.hpp"

#if defined(SX_ARCH_64_BIT)
    #define lseek64(fd, offset, whence) (off_t)lseek(fd, (off_t)offset, whence)
	#define stat64 stat
	#define fstat64 fstat
#endif

constexpr u32 FileCreatePermission = 0664;

Result File::Open(const char *filename, Mode mode, bool create){
    assert(m_FD == InvalidFD);

    errno = 0;

    m_FD = open(filename, (create ? O_CREAT : 0) | (int)mode, FileCreatePermission);

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
    m_Mode = Mode::Read;
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

i64 File::Seek(SeekPos position, i64 offset){
    assert(m_FD != InvalidFD);

    return lseek64(m_FD, offset, position);
}

i64 File::Tell(){
    assert(m_FD != InvalidFD);

    return lseek64(m_FD, 0, SEEK_CUR);
}

u64 File::Size(){
    assert(m_FD != InvalidFD);

    struct stat64 st;
    fstat64(m_FD, &st);
    return st.st_size;
}

Result File::Delete(const char *filename){
    return ResultError(unlink(filename) == -1);
}

bool File::Exist(const char *filename){
    return access(filename, F_OK) == 0;
}