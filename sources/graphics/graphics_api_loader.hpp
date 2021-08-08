#ifndef STRAITX_GRAPHICS_API_LOADER_HPP
#define STRAITX_GRAPHICS_API_LOADER_HPP

#include "core/result.hpp"
#include "graphics/graphics_context.hpp"

class GraphicsAPILoader{
public:
    static Result Load(GraphicsAPI api);
};

#endif//STRAITX_GRAPHICS_API_LOADER_HPP