#ifndef STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP
#define STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP

#include "platform/vulkan.hpp"
#include "core/push_array.hpp"
#include "graphics/api/framebuffer.hpp"

namespace StraitX{
namespace Vk{

class FramebufferImpl: public Framebuffer{
private:
    VkFramebuffer m_Handle = VK_NULL_HANDLE;
    PushArray<const GPUTexture *, MaxAttachmentsCount> m_Attachments;
public:
    FramebufferImpl(const RenderPass *const pass, const FramebufferProperties &props);

    ~FramebufferImpl();

    VkFramebuffer Handle()const{
        return m_Handle;
    }

    const PushArray<const GPUTexture *, MaxAttachmentsCount> &Attachments()const{
        return m_Attachments;
    }

    static Framebuffer *NewImpl(const RenderPass *const pass, const FramebufferProperties &props);

    static void DeleteImpl(Framebuffer *framebuffer);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP