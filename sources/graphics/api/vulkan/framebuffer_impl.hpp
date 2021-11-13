#ifndef STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP
#define STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP

#include "core/fixed_list.hpp"
#include "graphics/api/vulkan/debug.hpp"
#include "graphics/api/framebuffer.hpp"

namespace Vk{

class FramebufferImpl: public Framebuffer{
private:
    VkFramebuffer m_Handle = VK_NULL_HANDLE;
    FixedList<Texture2D *, MaxAttachmentsCount> m_Attachments;
    Vector2u m_Size;
public:
    FramebufferImpl(const FramebufferProperties &props);

    virtual ~FramebufferImpl();

    Vector2u Size()const override;

    ConstSpan<Texture2D *> Attachments()const override;

    operator VkFramebuffer()const{
        return m_Handle;
    }
};

}//namespace Vk::

#endif//STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP