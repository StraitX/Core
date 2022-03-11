#include "graphics/render_texture.hpp"

RenderTexture::RenderTexture(Vector2u size, TextureFormat color_format, TextureFormat depth_format):
	m_ColorFormat(color_format),
	m_DepthFormat(depth_format),
	m_Framebuffer(nullptr),
	m_Pass(nullptr)
{
	SX_ASSERT(IsColorFormat(color_format));
	FixedList<AttachmentDescription, 2> descriptions;
	
	Texture2D *color_attachment = Texture2D::Create(
		size, 
		color_format,
		TextureUsageBits::ColorAttachmentOptimal | TextureUsageBits::Sampled | TextureUsageBits::TransferDst, 
		TextureLayout::ShaderReadOnlyOptimal
	);

	m_Attachments.Emplace(color_attachment);
	descriptions.Add({
		TextureLayout::ShaderReadOnlyOptimal,
		TextureLayout::ColorAttachmentOptimal,
		TextureLayout::ShaderReadOnlyOptimal,
		color_attachment->Format(),
		SamplePoints::Samples_1
	});

	if(IsDepthFormat(depth_format)){
		Texture2D* depth_attachment = Texture2D::Create(
			size,
			depth_format,
			TextureUsageBits::DepthStencilOptimal | TextureUsageBits::TransferDst,
			TextureLayout::DepthStencilAttachmentOptimal
		);

		m_Attachments.Emplace(depth_attachment);
		descriptions.Add({
			TextureLayout::DepthStencilAttachmentOptimal,
			TextureLayout::DepthStencilAttachmentOptimal,
			TextureLayout::DepthStencilAttachmentOptimal,
			depth_attachment->Format(),
			SamplePoints::Samples_1
		});	
	}
	
	m_Pass = UniquePtr<class RenderPass>(RenderPass::Create({descriptions}));

	FixedList<Texture2D *, 2> attachments;
	for(auto &tex: m_Attachments)
		attachments.Add(tex.Get());

	m_Framebuffer = UniquePtr<class Framebuffer>(
		Framebuffer::Create({
			size,
			attachments,
			m_Pass.Get()
		})
	);
}

void RenderTexture::Resize(Vector2u size){
	if(Size() == size)
		return;

	TextureFormat color = m_ColorFormat;
	TextureFormat depth = m_DepthFormat;

	this->~RenderTexture();
	new(this) RenderTexture(size, color, depth);
}
