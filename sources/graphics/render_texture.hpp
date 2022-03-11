#pragma once

#include "core/unique_ptr.hpp"
#include "core/fixed_list.hpp"
#include "graphics/api/framebuffer.hpp"

class RenderTexture {
private:
	TextureFormat m_ColorFormat;
	TextureFormat m_DepthFormat;
	FixedList<UniquePtr<Texture2D>, 2> m_Attachments;
	UniquePtr<RenderPass> m_Pass;
	UniquePtr<Framebuffer> m_Framebuffer;
public:
	RenderTexture(Vector2u size, TextureFormat color_format, TextureFormat depth_format);

	void Resize(Vector2u size);
	
	Vector2u Size()const {
		return m_Framebuffer->Size();
	}
	
	const class Framebuffer* Framebuffer()const {
		return m_Framebuffer.Get();
	}

	const class RenderPass* RenderPass()const {
		return m_Pass.Get();
	}

	const Texture2D* ColorAttachment()const {
		SX_CORE_ASSERT(m_Attachments.Size() && m_Attachments[0] && IsColorFormat(m_Attachments[0]->Format()), "color attachment was not created");
		return m_Attachments[0].Get();
	}

	const Texture2D* DepthAttachment()const {
		SX_CORE_ASSERT(m_Attachments.Size() == 2 && m_Attachments[1] && IsDepthFormat(m_Attachments[1]->Format()), "depth attachment was not created");
		return m_Attachments[1].Get();
	}
};