#ifndef STRAITX_GPU_BUFFER_HPP
#define STRAITX_GPU_BUFFER_HPP

#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/logical_gpu.hpp"

namespace StraitX{

struct GPUBuffer{
    enum class AccessType{
        Dynamic,
        Static
    };

    enum class UsageType{
        VertexBuffer,
        IndexBuffer,
        UniformBuffer,
        TransferSource,
        TransferDestination
    };

    struct VTable{
        typedef Result (*CreateProc)(GPUBuffer&, LogicalGPU& ,u32, AccessType, UsageType);
        typedef void (*DestroyProc)(GPUBuffer&);

        CreateProc Create = 0;
        DestroyProc Destroy = 0;
    };

    VTable VirtualTable;
    LogicalGPU *m_Owner = nullptr;
    u32 m_Size = 0;
    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_BackingMemory = {};

    GPUBuffer();

    sx_inline Result Create(LogicalGPU &owner, u32 size, AccessType access, UsageType usage){
        return VirtualTable.Create(*this, owner, size, access, usage);
    }

    sx_inline void Destroy(){
        VirtualTable.Destroy(*this);
    }

};

}//namespace StraitX::

#endif//STRAITX_GPU_BUFFER_HPP