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
    static GraphicsAPIBackendImpl *s_Impl;
    static GraphicsAPIBackend s_Backend;
public:
    static Result CreateBackend();

    static void DestroyBackend();

    static bool HasBackend(){
        return s_Impl;
    }

    static GraphicsAPIBackend Backend(){
        return s_Backend;
    }
};

#endif//STRAITX_GRAPHICS_API_HPP