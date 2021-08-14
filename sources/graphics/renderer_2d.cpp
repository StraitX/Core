#include "core/raw_var.hpp"
#include "graphics/graphics_context.hpp"
#include "graphics/renderer_2d.hpp"
#include "graphics/batch_renderer_2d.hpp"

const Vector2f (&Renderer2D::s_DefaultTextureCoords)[4] = BatchRenderer2D::s_DefaultTextureCoords;

static RawVar<BatchRenderer2D> s_Renderer2DImpl;

void Renderer2D::Register(EngineDelegates &delegates){
	delegates.OnBeginFrame.Bind(OnBeginFrame);
	delegates.OnEndFrame.Bind(OnEndFrame);
	delegates.OnFinalize.Bind(OnFinalize);

	Initialize();
}

void Renderer2D::Initialize(){
	s_Renderer2DImpl.Construct(GraphicsContext::Get().FramebufferPass());
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

void Renderer2D::DrawRect(Vector2s position, Vector2s size, const Color &color, const Texture2D &texture, const Vector2f (&texture_coords)[4]){
	s_Renderer2DImpl->DrawRect(position, size, color, texture, texture_coords);
}

void Renderer2D::DrawRect(Vector2s position, Vector2s size, const Color &color){
	s_Renderer2DImpl->DrawRect(position, size, color);
}

void Renderer2D::DrawRect(Vector2s position, Vector2s size, const Texture2D &texture, const Vector2f (&texture_coords)[4]){
	s_Renderer2DImpl->DrawRect(position, size, texture, texture_coords);
}

void Renderer2D::Clear(const Color &color){
	s_Renderer2DImpl->Clear(color);
}
