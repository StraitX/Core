#ifndef STRAITX_CPU_BUFFER_HPP
#define STRAITX_CPU_BUFFER_HPP

#include "platform/result.hpp"
#include "platform/types.hpp"
#include "platform/compiler.hpp"
#include "core/assert.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/logical_gpu.hpp"

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

    LogicalGPU *const m_Owner = nullptr;
    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_BackingMemory = {};
    void *m_Pointer = nullptr;
    u32 m_Size = 0;

    friend class GL::CPUBufferImpl;
    friend class Vk::CPUBufferImpl;
    friend class GraphicsAPILoader;
public:
    sx_inline CPUBuffer();

#ifdef SX_DEBUG
    sx_inline ~CPUBuffer();
#endif
    sx_inline void New(u32 size);

    sx_inline void Delete();

    constexpr void *Pointer()const;

    void CopyData(const void *source, size_t size);

    constexpr GPUResourceHandle Handle()const;

    constexpr u32 Size()const;
    
};

sx_inline CPUBuffer::CPUBuffer():
    m_Owner(&LogicalGPU::Instance())
{}

// Use destructor to avoid Buffer leaks
#ifdef SX_DEBUG
CPUBuffer::~CPUBuffer(){
    CoreAssert(m_Pointer == nullptr, "CPUBuffer: Delete should be called before destruction");
}
#endif

sx_inline void CPUBuffer::New(u32 size){
    CoreAssert(m_Pointer == nullptr, "CPUBuffer: New() should be called on empty object");
    s_VTable.New(*this, size);
}

sx_inline void CPUBuffer::Delete(){
    s_VTable.Delete(*this);
#ifdef SX_DEBUG
    m_Pointer = nullptr;
#endif
}

constexpr void *CPUBuffer::Pointer()const{
    CoreAssert(m_Pointer, "CPUBuffer: For some reason Pointer is null");
    return m_Pointer;
}

constexpr GPUResourceHandle CPUBuffer::Handle()const{
    return m_Handle;
}

constexpr u32 CPUBuffer::Size()const{
    return m_Size;
}

}//namespace StraitX::

#endif//STRAITX_CPU_BUFFER_HPP