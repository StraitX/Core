#ifndef STRAITX_VULKAN_RENDER_PASS_IMPL
#define STRAITX_VULKAN_RENDER_PASS_IMPL

#include "core/os/vulkan.hpp"
#include "core/fixed_list.hpp"
#include "core/defs.hpp"
#include "graphics/api/render_pass.hpp"

namespace Vk{

class RenderPassImpl: public RenderPass{
private:
    VkRenderPass m_Handle = VK_NULL_HANDLE;
    FixedList<AttachmentDescription, MaxAttachmentsCount> m_Attachments;
    u32 m_DepthIndex = InvalidIndex;
public:
    RenderPassImpl(const RenderPassProperties &props);

    ~RenderPassImpl();

    ConstSpan<AttachmentDescription> Attachments()const override;

    bool HasDepth()const{
        return m_DepthIndex != InvalidIndex;
    }

    operator VkRenderPass()const{
        return m_Handle;
    }
};

}//namespace Vk::

#endif//STRAITX_VULKAN_RENDER_PASS_IMPL