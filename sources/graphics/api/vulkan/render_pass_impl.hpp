#ifndef STRAITX_VULKAN_RENDER_PASS_IMPL
#define STRAITX_VULKAN_RENDER_PASS_IMPL

#include "core/os/vulkan.hpp"
#include "core/push_array.hpp"
#include "core/defs.hpp"
#include "graphics/api/render_pass.hpp"

namespace Vk{

class RenderPassImpl: public RenderPass{
private:
    VkRenderPass m_Handle = VK_NULL_HANDLE;
    PushArray<AttachmentDescription, MaxAttachmentsCount> m_Attachments;
    u32 m_DepthIndex = InvalidIndex;
public:
    RenderPassImpl(const RenderPassProperties &props);

    ~RenderPassImpl();

    ConstSpan<AttachmentDescription> Attachments()const override;

    bool HasDepth()const{
        return m_DepthIndex != InvalidIndex;
    }
};

}//namespace Vk::

#endif//STRAITX_VULKAN_RENDER_PASS_IMPL