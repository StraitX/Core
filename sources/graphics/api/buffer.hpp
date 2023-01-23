#ifndef STRAITX_BUFFER_HPP
#define STRAITX_BUFFER_HPP

#include "core/types.hpp"
#include "core/unique_ptr.hpp"
#include "graphics/api/graphics_resource.hpp"

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
        StorageBuffer       = 0x20,
        IndexBuffer         = 0x40,
        VertexBuffer        = 0x80
    };
}//namespace BufferUsage::

class Buffer: public GraphicsResource{
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
    // Invalidates mappend pointer
    virtual void Realloc(size_t new_size) = 0;

    virtual size_t Size()const = 0;

    virtual BufferMemoryType MemoryType()const = 0;

    virtual BufferUsage Usage()const = 0;

    static Buffer *Create(size_t size, BufferMemoryType mem_type, BufferUsage usage);
    
    static Buffer* Create(const void* data, size_t size, BufferMemoryType mem_type, BufferUsage usage) {
        Buffer *buffer = Create(size, mem_type, usage);
        buffer->Copy(data, size);
        return buffer;
    }
};


template <typename ContentStructType>
class StructBuffer{
private:
    UniquePtr<Buffer> m_StructBuffer;
    UniquePtr<Buffer> m_StagingBuffer;
    ContentStructType *m_StagingDataPtr{m_StagingBuffer->Map<ContentStructType>()};

    friend class CommandBuffer;
public:
    StructBuffer():
        m_StructBuffer(
            Buffer::Create(
                sizeof(ContentStructType), 
                BufferMemoryType::DynamicVRAM, 
                BufferUsageBits::UniformBuffer | BufferUsageBits::TransferDestination
            )
        ),
        m_StagingBuffer(
            Buffer::Create(
                sizeof(ContentStructType), 
                BufferMemoryType::UncachedRAM, 
                BufferUsageBits::TransferSource
            )
        )
    {}

    void Copy(const ContentStructType& content){
        *m_StructBuffer->Map<ContentStructType>() = content;
        m_StructBuffer->Unmap();
    }
    
    operator const Buffer*()const{
        return m_StructBuffer.Get();
    }
};

#endif//STRAITX_BUFFER_HPP
