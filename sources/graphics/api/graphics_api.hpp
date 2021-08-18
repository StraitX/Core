#ifndef STRAITX_GRAPHICS_API_HPP
#define STRAITX_GRAPHICS_API_HPP

#include "core/result.hpp"

enum class GraphicsAPIBackend{
    None = 0,
    Vulkan,
    OpenGL
};

class GraphicsAPIBackendImpl{ 
public:
    virtual Result Create() = 0;

    virtual void Destroy() = 0;
};

class GraphicsAPI{
private:
    static GraphicsAPIBackendImpl *s_Backend;
public:
    static Result CreateBackend(GraphicsAPIBackend backend);

    static void DestroyBackend();
};

#endif//STRAITX_GRAPHICS_API_HPP