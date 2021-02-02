#ifndef STRAITX_GRAPHICS_API_HPP
#define STRAITX_GRAPHICS_API_HPP

#include "platform/types.hpp"
#include "platform/result.hpp"
#include "core/assert.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/physical_gpu.hpp"

namespace StraitX{

class Engine;

class GraphicsAPI: NonCopyable{
public:
    enum API{
        None = 0,
        Vulkan,
        OpenGL
    };
private:
    static GraphicsAPI *s_Instance;

    static API s_CurrentAPI;
private:
    static Result Create(API api);

    friend class Engine;
public:
    static sx_inline GraphicsAPI &Instance(){
        CoreAssert(s_Instance, "GraphicsAPI: has not been initialized for some reason");
        return *s_Instance;
    }

    static sx_inline API CurrentAPI(){
        return s_CurrentAPI;
    }

    virtual Result Initialize() = 0;

    virtual void Finalize() = 0;

    virtual u32 GetPhysicalGPUCount() = 0;

    virtual Result GetPhysicalGPUs(PhysicalGPU *array) = 0;

};

}// namepsace StraitX::

#endif //STRAITX_GRAPHICS_API_HPP 