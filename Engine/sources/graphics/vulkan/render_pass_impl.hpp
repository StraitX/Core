#ifndef STRAITX_VULKAN_RENDER_PASS_IMPL_HPP
#define STRAITX_VULKAN_RENDER_PASS_IMPL_HPP

#include "platform/vulkan.hpp"
#include "platform/defs.hpp"
#include "graphics/api/render_pass.hpp"

namespace StraitX{
namespace Vk{

class RenderPassImpl: public RenderPass{
private:
    VkRenderPass m_Handle = VK_NULL_HANDLE;
    PushArray<AttachmentDescription, MaxAttachmentsCount> m_Attachments;
    s32 m_DepthIndex = InvalidIndex;
public:
    RenderPassImpl(const RenderPassProperties &props);

    virtual ~RenderPassImpl()override;

    bool HasDepth()const;

    VkRenderPass Handle()const;

    const PushArray<AttachmentDescription, MaxAttachmentsCount> &Attachments()const;

    static RenderPass *NewImpl(const RenderPassProperties &props);
    
    static void DeleteImpl(RenderPass *pass);
};

sx_inline bool RenderPassImpl::HasDepth()const{
    return m_DepthIndex != InvalidIndex;
}

sx_inline VkRenderPass RenderPassImpl::Handle()const{
    return m_Handle;
}

sx_inline const PushArray<AttachmentDescription, MaxAttachmentsCount> &RenderPassImpl::Attachments()const{
    return m_Attachments;
}


}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_RENDER_PASS_IMPL_HPP