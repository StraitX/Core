#ifndef STRAITX_GRAPHICS_API_LOADER_HPP
#define STRAITX_GRAPHICS_API_LOADER_HPP

#include "platform/result.hpp"
#include "graphics/api/graphics_context.hpp"

class GraphicsAPILoader{
public:
    static Result Load(GraphicsAPI api);
};

#endif//STRAITX_GRAPHICS_API_LOADER_HPP