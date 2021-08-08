#ifndef STRAITX_CPU_BUFFER_HPP
#define STRAITX_CPU_BUFFER_HPP

#include "platform/result.hpp"
#include "platform/types.hpp"
#include "platform/compiler.hpp"
#include "core/assert.hpp"
#include "core/noncopyable.hpp"
#include "core/move.hpp"
#include "graphics/gpu_configuration.hpp"

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

    CPUBuffer(u32 size);

    CPUBuffer(u32 size, const void *data);

    CPUBuffer(CPUBuffer &&other)noexcept;

    ~CPUBuffer();

    CPUBuffer &operator=(CPUBuffer &&other)noexcept;

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

SX_INLINE CPUBuffer::CPUBuffer(u32 size){
    New(size);
}

SX_INLINE CPUBuffer::CPUBuffer(u32 size, const void *data){
    New(size, data);
}

SX_INLINE CPUBuffer::CPUBuffer(CPUBuffer &&other)noexcept{
    *this = Move(other);
}

SX_INLINE CPUBuffer::~CPUBuffer(){
    if(!IsEmpty())
        Delete();
}

SX_INLINE CPUBuffer &CPUBuffer::operator=(CPUBuffer &&other)noexcept{
    SX_CORE_ASSERT(IsEmpty(), "CPUBuffer: Can't move into non-empty object");
    m_Handle = other.m_Handle;
    m_BackingMemory = other.m_BackingMemory;
    m_Pointer = other.m_Pointer;
    m_Size = other.m_Size;

    other.SetZero();
    return *this;
}


SX_INLINE void CPUBuffer::New(u32 size){
    SX_CORE_ASSERT(IsEmpty(), "CPUBuffer: New() should be called on empty object");
    s_VTable.New(*this, size);
}

SX_INLINE void CPUBuffer::New(u32 size, const void *data){
    New(size);
    CopyData(data, size);
}

SX_INLINE void CPUBuffer::Delete(){
    s_VTable.Delete(*this);
    SetZero();
}

SX_INLINE void *CPUBuffer::Pointer()const{
    SX_CORE_ASSERT(m_Pointer, "CPUBuffer: For some reason Pointer is null");
    return m_Pointer;
}

SX_INLINE GPUResourceHandle CPUBuffer::Handle()const{
    return m_Handle;
}

SX_INLINE u32 CPUBuffer::Size()const{
    return m_Size;
}

SX_INLINE bool CPUBuffer::IsEmpty()const{
    return m_Handle.U64 == 0;
}

SX_INLINE void CPUBuffer::SetZero(){
    m_Handle = {};
    m_BackingMemory = {};
    m_Pointer = nullptr;
    m_Size = 0;
}

#endif//STRAITX_CPU_BUFFER_HPP