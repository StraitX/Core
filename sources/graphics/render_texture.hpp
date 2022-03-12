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
		for (const auto& attach : m_Attachments) {
			if(IsColorFormat(attach->Format()))
				return attach.Get();
		}
		SX_CORE_ASSERT(false, "color attachment was not created");
		return nullptr;
	}

	const Texture2D* DepthAttachment()const {
		for (const auto& attach : m_Attachments) {
			if(IsDepthFormat(attach->Format()))
				return attach.Get();
		}
		SX_CORE_ASSERT(false, "depth attachment was not created");
		return nullptr;
	}
};