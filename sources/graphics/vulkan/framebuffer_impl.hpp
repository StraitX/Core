#ifndef STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP
#define STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP

#include "platform/vulkan.hpp"
#include "core/push_array.hpp"
#include "graphics/framebuffer.hpp"

namespace Vk{

class FramebufferImpl: public Framebuffer{
private:
    VkFramebuffer m_Handle = VK_NULL_HANDLE;
    PushArray<const Texture2D *, MaxAttachmentsCount> m_Attachments;
	Vector2u m_Size;
public:
    FramebufferImpl(const RenderPass *const pass, const FramebufferProperties &props);

    ~FramebufferImpl();

    VkFramebuffer Handle()const{
        return m_Handle;
    }

	virtual Vector2u Size()const override{
		return m_Size;
	}

    const PushArray<const Texture2D *, MaxAttachmentsCount> &Attachments()const{
        return m_Attachments;
    }

    static Framebuffer *NewImpl(const RenderPass *const pass, const FramebufferProperties &props);

    static void DeleteImpl(Framebuffer *framebuffer);
};

}//namespace Vk::

#endif//STRAITX_VULKAN_FRAMEBUFFER_IMPL_HPP