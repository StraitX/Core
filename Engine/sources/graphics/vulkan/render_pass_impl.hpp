#ifndef STRAITX_VULKAN_RENDER_PASS_IMPL_HPP
#define STRAITX_VULKAN_RENDER_PASS_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/render_pass.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct RenderPassImpl: RenderPass{
    const Vk::LogicalGPUImpl *Owner;
    VkRenderPass Handle = VK_NULL_HANDLE;
    PushArray<AttachmentDescription, MaxAttachmentsCount> Attachments;
    i32 DepthIndex = InvalidIndex;

    RenderPassImpl(const LogicalGPU &owner, const RenderPassProperties &props);

    virtual ~RenderPassImpl()override;

    constexpr bool HasDepth()const;

    static RenderPass *NewImpl(const LogicalGPU &owner, const RenderPassProperties &props);
    
    static void DeleteImpl(RenderPass *pass);
};

constexpr bool RenderPassImpl::HasDepth()const{
    return DepthIndex != InvalidIndex;
}

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_RENDER_PASS_IMPL_HPP