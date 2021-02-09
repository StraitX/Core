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


class CPUBuffer: NonCopyable{
private:
    LogicalGPU *const m_Owner = nullptr;
    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_BackingMemory = {};
    void *m_Pointer = nullptr;
    size_t m_Size = 0;

    friend class GL::CPUBufferImpl;
    friend class Vk::CPUBufferImpl;
public:
    sx_inline CPUBuffer():
        m_Owner(&LogicalGPU::Instance())
    {}

// Use destructor to avoid Buffer leaks
#ifdef SX_DEBUG
    ~CPUBuffer(){
        CoreAssert(m_Pointer == nullptr, "CPUBuffer: Delete should be called before destruction");
    }
#endif

    sx_inline void New(size_t size){
        CoreAssert(m_Pointer == nullptr, "CPUBuffer: New() should be called on empty object");
        m_Owner->NewCPUBuffer(*this, size);
    }

    sx_inline void Delete(){
        m_Owner->DeleteCPUBuffer(*this);
#ifdef SX_DEBUG
        m_Pointer = nullptr;
#endif
    }

    sx_inline void *Pointer(){
        CoreAssert(m_Pointer, "CPUBuffer: For some reason Pointer is null");
        return m_Pointer;
    }

    void CopyData(const void *source, size_t size);
    
};

}//namespace StraitX::

#endif//STRAITX_CPU_BUFFER_HPP