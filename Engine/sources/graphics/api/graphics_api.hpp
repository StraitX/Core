#ifndef STRAITX_GRAPHICS_API_HPP
#define STRAITX_GRAPHICS_API_HPP

#include "platform/types.hpp"
#include "platform/result.hpp"
#include "core/assert.hpp"
#include "core/noncopyable.hpp"

namespace StraitX{

class GraphicsAPI: public NonCopyable{
public:
    enum API{
        None = 0,
        Vulkan,
        OpenGL
    };
private:
    static GraphicsAPI *s_Instance;
    static API s_CurrentAPI;

    friend class RenderWindow;
    friend class GraphicsAPILoader;
private:
    virtual Result Initialize() = 0;

    virtual void Finalize() = 0;
public:
    static API CurrentAPI();

    static const char *Name(GraphicsAPI::API api);
};

sx_inline GraphicsAPI::API GraphicsAPI::CurrentAPI(){
    return s_CurrentAPI;
}

}// namespace StraitX::

#endif //STRAITX_GRAPHICS_API_HPP 