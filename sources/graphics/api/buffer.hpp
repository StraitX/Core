#ifndef STRAITX_BUFFER_HPP
#define STRAITX_BUFFER_HPP

#include "core/types.hpp"
#include "core/noncopyable.hpp"

enum class BufferMemoryType{
    VRAM,
    DynamicVRAM,
    RAM,
    UncachedRAM
};

using BufferUsage = u8;

namespace BufferUsageBits{
    enum Values: BufferUsage{ // don't mess them up, these are tied to vulkan spec
        TransferSource      = 0x01,      
        TransferDestination = 0x02,
        //UniformTexelBuffer  = 0x04,
        //StorageTexelBuffer  = 0x08,
        UniformBuffer       = 0x10,
        //StorageBuffer       = 0x20,
        IndexBuffer         = 0x40,
        VertexBuffer        = 0x80
    };
}//namespace BufferUsage::

class Buffer: public NonCopyable{
public:
    virtual ~Buffer() = default;

    virtual void Copy(Buffer *src, size_t size, size_t src_offset = 0, size_t dst_offset = 0) = 0;

    virtual void Copy(const void *data, size_t size, size_t offset = 0) = 0;

    virtual bool IsMappable()const = 0;

    virtual bool IsMapped()const = 0;

    virtual void *Map() = 0;

    template <typename Type>
    Type *Map(){
        return static_cast<Type*>(Map());
    }

    virtual void Unmap() = 0;

    virtual size_t Size()const = 0;

    virtual BufferMemoryType MemoryType()const = 0;

    virtual BufferUsage Usage()const = 0;

    static Buffer *Create(size_t size, BufferMemoryType mem_type, BufferUsage usage);
};

#endif//STRAITX_BUFFER_HPP