#include "core/raw_var.hpp"
#include "graphics/api/graphics_context.hpp"
#include "graphics/renderer_2d.hpp"
#include "graphics/batch_renderer_2d.hpp"

namespace StraitX{

const Vector2f (&Renderer2D::s_DefaultTextureCoords)[4] = BatchRenderer2D::s_DefaultTextureCoords;

static RawVar<BatchRenderer2D> s_Renderer2DImpl;

Renderer2D::Renderer2D():
	Subsystem("Renderer2D")
{}

Result Renderer2D::OnInitialize(){
	s_Renderer2DImpl.Construct(GraphicsContext::Get().FramebufferPass());
	return Result::Success;
}

void Renderer2D::OnFinalize(){
	s_Renderer2DImpl.Destruct();
}

void Renderer2D::OnBeginFrame(){
	s_Renderer2DImpl->BeginScene(GraphicsContext::Get().CurrentFramebuffer());
}

void Renderer2D::OnEndFrame(){
	s_Renderer2DImpl->EndScene();
}

void Renderer2D::DrawRect(Vector2i position, Vector2i size, const Color &color, const Texture2D &texture, const Vector2f (&texture_coords)[4]){
	s_Renderer2DImpl->DrawRect(position, size, color, texture, texture_coords);
}

void Renderer2D::DrawRect(Vector2i position, Vector2i size, const Color &color){
	s_Renderer2DImpl->DrawRect(position, size, color);
}

void Renderer2D::DrawRect(Vector2i position, Vector2i size, const Texture2D &texture, const Vector2f (&texture_coords)[4]){
	s_Renderer2DImpl->DrawRect(position, size, texture, texture_coords);
}

void Renderer2D::Clear(const Color &color){
	s_Renderer2DImpl->Clear(color);
}

}//namespace StraitX::