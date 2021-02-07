#ifndef STRAITX_GPU_BUFFER_HPP
#define STRAITX_GPU_BUFFER_HPP

#include "core/noncopyable.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/logical_gpu.hpp"

namespace StraitX{

namespace GL{
class GPUBufferImpl;
}//namesapce GL::

namespace Vk{
class GPUBufferImpl;
}//namespace Vk::

class GPUBuffer: NonCopyable{
public:
    enum UsageType: u8{ // don't mess them up, these are tied to vulkan spec
        TransferSource      = 0x01,      
        TransferDestination = 0x02,
        //UniformTexelBuffer  = 0x04,
        //StorageTextlBuffer  = 0x08,
        UniformBuffer       = 0x10,
        //StorageBuffer       = 0x20,
        IndexBuffer         = 0x40,
        VertexBuffer        = 0x80
    };
private:
    LogicalGPU *m_Owner = nullptr;
    u32 m_Size          = 0;
    GPUResourceHandle m_Handle          = {};
    GPUResourceHandle m_BackingMemory   = {};

    friend class GL::GPUBufferImpl;
    friend class Vk::GPUBufferImpl;
public:
    GPUBuffer();

    Result Create(LogicalGPU &owner, u32 size, GPUMemoryType mem_type, UsageType usage);

    void Destroy();

};

}//namespace StraitX::

#endif//STRAITX_GPU_BUFFER_HPP