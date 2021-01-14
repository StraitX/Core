#ifndef STRAITX_GRAPHICS_API_HPP
#define STRAITX_GRAPHICS_API_HPP

#include "platform/types.hpp"
#include "platform/result.hpp"
#include "core/assert.hpp"
#include "graphics/api/physical_gpu.hpp"

namespace StraitX{

class Engine;

class GraphicsAPI{
public:
    enum API{
        None = 0,
        Vulkan,
        OpenGL
    };
private:
    friend class Engine;

    static GraphicsAPI *s_Instance;

    static API s_CurrentAPI;
private:
    static Result Create(API api);
public:
    static sx_inline GraphicsAPI &Instance(){
        CoreAssert(s_Instance, "GraphicsAPI: has not been initialized for some reason");
        return *s_Instance;
    }

    static sx_inline API CurrentAPI(){
        return s_CurrentAPI;
    }

    virtual Result Initialize() = 0;

    virtual Result Finalize() = 0;

    virtual u32 GetPhysicalGPUCount() = 0;

    virtual Result GetPhysicalGPUs(PhysicalGPU *array) = 0;

};

}// namepsace StraitX::

#endif //STRAITX_GRAPHICS_API_HPP 