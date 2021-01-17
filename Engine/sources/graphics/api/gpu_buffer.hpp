#ifndef STRAITX_GPU_BUFFER_HPP
#define STRAITX_GPU_BUFFER_HPP

#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/logical_gpu.hpp"

namespace StraitX{

namespace GL{
class GPUBufferImpl;
}//namesapce GL::

namespace Vk{
class GPUBufferImpl;
}//namespace Vk::

class GPUBuffer{
public:
    enum UsageType: u8{ // don't mess them up, these are tied to vulkan spec
        TransferSource      = 0x01,      
        TransferDestination = 0x02,
        UniformTexelBuffer  = 0x04,
        StorageTextlBuffer  = 0x08,
        UniformBuffer       = 0x10,
        StorageBuffer       = 0x20,
        IndexBuffer         = 0x40,
        VertexBuffer        = 0x80
    };

    struct VTable{
        typedef Result (*CreateProc)(GPUBuffer&, LogicalGPU& ,u32, GPUMemoryType, UsageType);
        typedef void (*DestroyProc)(GPUBuffer&);

        CreateProc Create = 0;
        DestroyProc Destroy = 0;
    };
private:
    const VTable VirtualTable;
    LogicalGPU *m_Owner = nullptr;
    u32 m_Size          = 0;
    GPUResourceHandle m_Handle          = {};
    GPUResourceHandle m_BackingMemory   = {};

    friend class GL::GPUBufferImpl;
    friend class Vk::GPUBufferImpl;
public:
    GPUBuffer();

    sx_inline Result Create(LogicalGPU &owner, u32 size, GPUMemoryType mem_type, UsageType usage){
        return VirtualTable.Create(*this, owner, size, mem_type, usage);
    }

    sx_inline void Destroy(){
        VirtualTable.Destroy(*this);
    }

    sx_inline u32 Size(){
        return m_Size;
    }

};

}//namespace StraitX::

#endif//STRAITX_GPU_BUFFER_HPP