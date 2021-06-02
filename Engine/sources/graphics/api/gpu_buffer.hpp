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

    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_BackingMemory = {};
    u32 m_Size = 0;
    UsageType m_Usage = {};
    GPUMemoryType m_MemoryType = {};

    friend class GraphicsAPILoader;
    friend class GL::GPUBufferImpl;
    friend class Vk::GPUBufferImpl;
public:
    GPUBuffer() = default;

    GPUBuffer(GPUBuffer &&other);

    ~GPUBuffer();

    GPUBuffer &operator=(GPUBuffer &&other);

    void New(u32 size, GPUMemoryType mem_type, UsageType usage);

    void Delete();

    GPUResourceHandle Handle()const;

    GPUResourceHandle Memory()const;

    u32 Size()const;

    UsageType Usage()const;

    GPUMemoryType MemoryType()const;

    bool IsEmpty()const;
private:
    void SetZero();
};

sx_inline GPUBuffer::GPUBuffer(GPUBuffer &&other){
    *this = Move(other);
}

sx_inline GPUBuffer::~GPUBuffer(){
    if(!IsEmpty())
        Delete();
}

sx_inline GPUBuffer &GPUBuffer::operator=(GPUBuffer &&other){
    SX_CORE_ASSERT(IsEmpty(), "GPUBuffer: Can't move into non-empty object");
    m_Size = other.m_Size;
    m_Handle = other.m_Handle;
    m_BackingMemory = other.m_BackingMemory;
    m_Usage = other.m_Usage;
    m_MemoryType = other.m_MemoryType;
    other.SetZero();
    return *this;
}

sx_inline void GPUBuffer::New(u32 size, GPUMemoryType mem_type, UsageType usage){
    SX_CORE_ASSERT(IsEmpty(), "GPUBuffer: New() should be called on empty object");
    s_VTable.New(*this, size, mem_type, usage);
}

sx_inline void GPUBuffer::Delete(){
    s_VTable.Delete(*this);
    SetZero();
}

sx_inline GPUResourceHandle GPUBuffer::Handle()const{
    return m_Handle;
}

sx_inline GPUResourceHandle GPUBuffer::Memory()const{
    return m_BackingMemory;
}

sx_inline u32 GPUBuffer::Size()const{
    return m_Size;
}

sx_inline GPUBuffer::UsageType GPUBuffer::Usage()const{
    return m_Usage;
}

sx_inline GPUMemoryType GPUBuffer::MemoryType()const{
    return m_MemoryType;
}

sx_inline bool GPUBuffer::IsEmpty()const{
    return m_Handle.U64 == 0;
}

sx_inline void GPUBuffer::SetZero(){
    m_Size = 0;
    m_Handle = {};
    m_BackingMemory = {};
    m_Usage = {};
    m_MemoryType = {};
}

}//namespace StraitX::

#endif//STRAITX_GPU_BUFFER_HPP