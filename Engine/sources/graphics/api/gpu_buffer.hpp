#ifndef STRAITX_GPU_BUFFER_HPP
#define STRAITX_GPU_BUFFER_HPP

#include "platform/compiler.hpp"
#include "core/noncopyable.hpp"
#include "core/algorithm.hpp"
#include "core/assert.hpp"
#include "graphics/api/gpu_configuration.hpp"

namespace StraitX{

namespace GL{
class GPUBufferImpl;
}//namespace GL::

namespace Vk{
class GPUBufferImpl;
}//namespace Vk::

class GraphicsAPILoader;

class GPUBuffer: public NonCopyable{
public:
    enum UsageTypeBits: u8{ // don't mess them up, these are tied to vulkan spec
        TransferSource      = 0x01,      
        TransferDestination = 0x02,
        //UniformTexelBuffer  = 0x04,
        //StorageTexelBuffer  = 0x08,
        UniformBuffer       = 0x10,
        //StorageBuffer       = 0x20,
        IndexBuffer         = 0x40,
        VertexBuffer        = 0x80
    };
    using UsageType = u8;

    struct VTable{
        using NewProc    = void (*)(GPUBuffer &buffer, u32 size, GPUMemoryType mem_type, UsageType usage);
        using DeleteProc = void (*)(GPUBuffer &buffer);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;

    u32 m_Size = 0;
    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_BackingMemory = {};
    UsageType m_Usage = {};
    GPUMemoryType m_MemoryType = {};

    friend class GraphicsAPILoader;
    friend class GL::GPUBufferImpl;
    friend class Vk::GPUBufferImpl;
public:
    constexpr GPUBuffer() = default;
#ifdef SX_DEBUG
    sx_inline ~GPUBuffer();
#endif
    sx_inline void New(u32 size, GPUMemoryType mem_type, UsageType usage);

    sx_inline void Delete();

    constexpr GPUResourceHandle Handle()const;

    constexpr GPUResourceHandle Memory()const;

    constexpr u32 Size()const;

    constexpr UsageType Usage()const;

    constexpr GPUMemoryType MemoryType()const;

};
// Use destructor to avoid Buffer leaks
#ifdef SX_DEBUG
GPUBuffer::~GPUBuffer(){
    CoreAssert(m_Handle.U64 == 0, "GPUBuffer: Delete() should be called before destruction");
}
#endif

sx_inline void GPUBuffer::New(u32 size, GPUMemoryType mem_type, UsageType usage){
    CoreAssert(m_Handle.U64 == 0, "GPUBuffer: New() should be called on empty object");
    s_VTable.New(*this, size, mem_type, usage);
}

sx_inline void GPUBuffer::Delete(){
    s_VTable.Delete(*this);
    m_Handle.U64 = 0;
}

constexpr GPUResourceHandle GPUBuffer::Handle()const{
    return m_Handle;
}

constexpr GPUResourceHandle GPUBuffer::Memory()const{
    return m_BackingMemory;
}

constexpr u32 GPUBuffer::Size()const{
    return m_Size;
}

constexpr GPUBuffer::UsageType GPUBuffer::Usage()const{
    return m_Usage;
}

constexpr GPUMemoryType GPUBuffer::MemoryType()const{
    return m_MemoryType;
}

}//namespace StraitX::

#endif//STRAITX_GPU_BUFFER_HPP