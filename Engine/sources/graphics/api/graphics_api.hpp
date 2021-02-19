#ifndef STRAITX_GRAPHICS_API_HPP
#define STRAITX_GRAPHICS_API_HPP

#include "platform/types.hpp"
#include "platform/result.hpp"
#include "core/assert.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/physical_gpu.hpp"

namespace StraitX{

class Engine;
class GraphicsAPILoader;

class GraphicsAPI: NonCopyable{
public:
    enum API{
        None = 0,
        Vulkan,
        OpenGL,

        ElementsCount
    };
private:
    static GraphicsAPI *s_Instance;
    static API s_CurrentAPI;
    friend class Engine;
    friend class GraphicsAPILoader;
public:
    sx_inline static GraphicsAPI &Instance();

    sx_inline static API CurrentAPI();

    virtual Result Initialize() = 0;

    virtual void Finalize() = 0;

    virtual u32 GetPhysicalGPUCount() = 0;

    virtual Result GetPhysicalGPUs(PhysicalGPU *array) = 0;

};

sx_inline GraphicsAPI &GraphicsAPI::Instance(){
    CoreAssert(s_Instance, "GraphicsAPI: has not been initialized for some reason");
    return *s_Instance;
}

sx_inline GraphicsAPI::API GraphicsAPI::CurrentAPI(){
    return s_CurrentAPI;
}

}// namespace StraitX::

#endif //STRAITX_GRAPHICS_API_HPP 