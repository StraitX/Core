#ifndef STRAITX_VULKAN_GPU_CONTEXT_IMPL_HPP
#define STRAITX_VULKAN_GPU_CONTEXT_IMPL_HPP

#include "graphics/api/gpu_context.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

class GPUContextImpl: public GPUContext{
private:
    Vk::LogicalGPUImpl *m_Owner = nullptr;
    VkCommandPool   m_CmdPool   = VK_NULL_HANDLE;
    VkCommandBuffer m_CmdBuffer = VK_NULL_HANDLE;
public:
    GPUContextImpl(Vk::LogicalGPUImpl *owner);

    ~GPUContextImpl();

    virtual void BeginFrame()override;

    virtual void EndFrame()override;

    virtual void Submit()override;

    virtual void SumbitAsync()override;

    static GPUContext *NewImpl(LogicalGPU &owner);

    static void DeleteImpl(GPUContext *context);

};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GPU_CONTEXT_IMPL_HPP