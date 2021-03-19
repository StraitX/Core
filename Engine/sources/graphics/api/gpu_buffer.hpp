#ifndef STRAITX_GPU_BUFFER_HPP
#define STRAITX_GPU_BUFFER_HPP

#include "platform/compiler.hpp"
#include "core/noncopyable.hpp"
#include "core/algorithm.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/logical_gpu.hpp"

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

    LogicalGPU *m_Owner = nullptr;
    u32 m_Size = 0;
    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_BackingMemory = {};
    UsageType m_Usage = {};

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

    constexpr LogicalGPU *Owner()const;

    constexpr GPUResourceHandle Handle()const;

    constexpr u32 Size()const;

    constexpr UsageType Usage()const;

};
// Use destructor to avoid Buffer leaks
#ifdef SX_DEBUG
GPUBuffer::~GPUBuffer(){
    CoreAssert(m_Handle.U64 == 0, "GPUBuffer: Delete() should be called before destruction");
}
#endif

sx_inline void GPUBuffer::New(u32 size, GPUMemoryType mem_type, UsageType usage){
    m_Owner = &LogicalGPU::Instance();
    CoreAssert(m_Handle.U64 == 0, "GPUBuffer: New() should be called on empty object");
    s_VTable.New(*this, size, mem_type, usage);
}

sx_inline void GPUBuffer::Delete(){
    s_VTable.Delete(*this);
#ifdef SX_DEBUG
    m_Handle.U64 = 0;
#endif
}

constexpr LogicalGPU *GPUBuffer::Owner()const{
    return m_Owner;
}

constexpr GPUResourceHandle GPUBuffer::Handle()const{
    return m_Handle;
}

constexpr u32 GPUBuffer::Size()const{
    return m_Size;
}

constexpr GPUBuffer::UsageType GPUBuffer::Usage()const{
    return m_Usage;
}

}//namespace StraitX::

#endif//STRAITX_GPU_BUFFER_HPP