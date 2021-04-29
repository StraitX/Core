#ifndef STRAITX_CPU_BUFFER_HPP
#define STRAITX_CPU_BUFFER_HPP

#include "platform/result.hpp"
#include "platform/types.hpp"
#include "platform/compiler.hpp"
#include "core/assert.hpp"
#include "core/noncopyable.hpp"
#include "core/move.hpp"
#include "graphics/api/gpu_configuration.hpp"

namespace StraitX{

namespace GL{
class CPUBufferImpl;
}//namespace GL::

namespace Vk{
class CPUBufferImpl;
}//namespace Vk::

class GraphicsAPILoader;

class CPUBuffer: public NonCopyable{
public:
    struct VTable{
        using NewProc    = void (*)(CPUBuffer &buffer, u32 size);
        using DeleteProc = void (*)(CPUBuffer &buffer);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;

    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_BackingMemory = {};
    void *m_Pointer = nullptr;
    u32 m_Size = 0;

    friend class GL::CPUBufferImpl;
    friend class Vk::CPUBufferImpl;
    friend class GraphicsAPILoader;
public:
    CPUBuffer() = default;

    CPUBuffer(CPUBuffer &&other);

    ~CPUBuffer();

    CPUBuffer &operator=(CPUBuffer &&other);

    void New(u32 size);

    void New(u32 size, const void *data);

    void Delete();

    void *Pointer()const;

    void CopyData(const void *source, size_t size);

    GPUResourceHandle Handle()const;

    u32 Size()const;

    bool IsEmpty()const;
private:
    void SetZero();
};


sx_inline CPUBuffer::CPUBuffer(CPUBuffer &&other){
    *this = Move(other);
}

sx_inline CPUBuffer::~CPUBuffer(){
    if(!IsEmpty())
        Delete();
}

sx_inline CPUBuffer &CPUBuffer::operator=(CPUBuffer &&other){
    CoreAssert(IsEmpty(), "CPUBuffer: Can't move into non-empty object");
    m_Handle = other.m_Handle;
    m_BackingMemory = other.m_BackingMemory;
    m_Pointer = other.m_Pointer;
    m_Size = other.m_Size;

    other.SetZero();
    return *this;
}


sx_inline void CPUBuffer::New(u32 size){
    CoreAssert(IsEmpty(), "CPUBuffer: New() should be called on empty object");
    s_VTable.New(*this, size);
}

sx_inline void CPUBuffer::New(u32 size, const void *data){
    New(size);
    CopyData(data, size);
}

sx_inline void CPUBuffer::Delete(){
    s_VTable.Delete(*this);
    SetZero();
}

sx_inline void *CPUBuffer::Pointer()const{
    CoreAssert(m_Pointer, "CPUBuffer: For some reason Pointer is null");
    return m_Pointer;
}

sx_inline GPUResourceHandle CPUBuffer::Handle()const{
    return m_Handle;
}

sx_inline u32 CPUBuffer::Size()const{
    return m_Size;
}

sx_inline bool CPUBuffer::IsEmpty()const{
    return m_Handle.U64 == 0;
}

sx_inline void CPUBuffer::SetZero(){
    m_Handle = {};
    m_BackingMemory = {};
    m_Pointer = nullptr;
    m_Size = 0;
}

}//namespace StraitX::

#endif//STRAITX_CPU_BUFFER_HPP