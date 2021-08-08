#ifndef STRAITX_GPU_BUFFER_HPP
#define STRAITX_GPU_BUFFER_HPP

#include "core/env/compiler.hpp"
#include "core/noncopyable.hpp"
#include "core/algorithm.hpp"
#include "core/assert.hpp"
#include "graphics/gpu_configuration.hpp"

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

    GPUBuffer(GPUBuffer &&other)noexcept;

    ~GPUBuffer();

    GPUBuffer &operator=(GPUBuffer &&other)noexcept;

    void New(u32 size, GPUMemoryType mem_type, UsageType usage);

    void Delete();

	void Realloc(u32 new_size);

    GPUResourceHandle Handle()const;

    GPUResourceHandle Memory()const;

    u32 Size()const;

    UsageType Usage()const;

    GPUMemoryType MemoryType()const;

    bool IsEmpty()const;
private:
    void SetZero();
};

SX_INLINE GPUBuffer::GPUBuffer(GPUBuffer &&other)noexcept{
    *this = Move(other);
}

SX_INLINE GPUBuffer::~GPUBuffer(){
    if(!IsEmpty())
        Delete();
}

SX_INLINE GPUBuffer &GPUBuffer::operator=(GPUBuffer &&other)noexcept{
    SX_CORE_ASSERT(IsEmpty(), "GPUBuffer: Can't move into non-empty object");
    m_Size = other.m_Size;
    m_Handle = other.m_Handle;
    m_BackingMemory = other.m_BackingMemory;
    m_Usage = other.m_Usage;
    m_MemoryType = other.m_MemoryType;
    other.SetZero();
    return *this;
}

SX_INLINE void GPUBuffer::New(u32 size, GPUMemoryType mem_type, UsageType usage){
    SX_CORE_ASSERT(IsEmpty(), "GPUBuffer: New() should be called on empty object");
    s_VTable.New(*this, size, mem_type, usage);
}

SX_INLINE void GPUBuffer::Delete(){
    s_VTable.Delete(*this);
    SetZero();
}

SX_INLINE void GPUBuffer::Realloc(u32 new_size){
    SX_CORE_ASSERT(!IsEmpty(), "GPUBuffer: Realloc() should be called on non-empty object");
	s_VTable.Delete(*this);
	s_VTable.New(*this, new_size, m_MemoryType, m_Usage);
}

SX_INLINE GPUResourceHandle GPUBuffer::Handle()const{
    return m_Handle;
}

SX_INLINE GPUResourceHandle GPUBuffer::Memory()const{
    return m_BackingMemory;
}

SX_INLINE u32 GPUBuffer::Size()const{
    return m_Size;
}

SX_INLINE GPUBuffer::UsageType GPUBuffer::Usage()const{
    return m_Usage;
}

SX_INLINE GPUMemoryType GPUBuffer::MemoryType()const{
    return m_MemoryType;
}

SX_INLINE bool GPUBuffer::IsEmpty()const{
    return m_Handle.U64 == 0;
}

SX_INLINE void GPUBuffer::SetZero(){
    m_Size = 0;
    m_Handle = {};
    m_BackingMemory = {};
    m_Usage = {};
    m_MemoryType = {};
}

#endif//STRAITX_GPU_BUFFER_HPP