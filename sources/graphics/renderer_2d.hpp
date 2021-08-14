#ifndef STRAITX_RENDERER_2D_HPP
#define STRAITX_RENDERER_2D_HPP

#include "core/math/vector2.hpp"
#include "graphics/color.hpp"
#include "graphics/texture.hpp"
#include "main/engine.hpp"

class Renderer2D{
private:
	static const Vector2f (&s_DefaultTextureCoords)[4];
private:
	friend class Engine;

	static void Register(EngineDelegates &delegates);

	static void Initialize();

	static void OnFinalize();

	static void OnBeginFrame();

	static void OnEndFrame();
public:
	static void DrawRect(Vector2s position, Vector2s size, const Color &color, const Texture2D &texture, const Vector2f (&texture_coords)[4] = s_DefaultTextureCoords);

    static void DrawRect(Vector2s position, Vector2s size, const Color &color);

    static void DrawRect(Vector2s position, Vector2s size, const Texture2D &texture, const Vector2f (&texture_coords)[4] = s_DefaultTextureCoords);

	static void Clear(const Color &color);
};

#endif//STRAITX_RENDERER_2D_HPP