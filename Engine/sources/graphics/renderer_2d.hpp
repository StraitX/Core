#ifndef STRAITX_RENDERER_2D_HPP
#define STRAITX_RENDERER_2D_HPP

#include "core/subsystem.hpp"
#include "core/math/vector2.hpp"
#include "graphics/color.hpp"
#include "graphics/api/texture.hpp"

class Renderer2D: public Subsystem{
private:
	static const Vector2f (&s_DefaultTextureCoords)[4];
private:
	friend class Engine;

	Renderer2D();

	Result OnInitialize()override;

	void OnFinalize()override;

	void OnBeginFrame()override;

	void OnEndFrame()override;
public:
	static void DrawRect(Vector2i position, Vector2i size, const Color &color, const Texture2D &texture, const Vector2f (&texture_coords)[4] = s_DefaultTextureCoords);

    static void DrawRect(Vector2i position, Vector2i size, const Color &color);

    static void DrawRect(Vector2i position, Vector2i size, const Texture2D &texture, const Vector2f (&texture_coords)[4] = s_DefaultTextureCoords);

	static void Clear(const Color &color);
};

#endif//STRAITX_RENDERER_2D_HPP