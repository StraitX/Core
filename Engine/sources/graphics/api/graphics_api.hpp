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

    friend class Engine;
    friend class GraphicsAPILoader;
public:
    static sx_inline GraphicsAPI &Instance(){
        CoreAssert(s_Instance, "GraphicsAPI: has not been initialized for some reason");
        return *s_Instance;
    }

    virtual Result Initialize() = 0;

    virtual void Finalize() = 0;

    virtual u32 GetPhysicalGPUCount() = 0;

    virtual Result GetPhysicalGPUs(PhysicalGPU *array) = 0;

};

}// namepsace StraitX::

#endif //STRAITX_GRAPHICS_API_HPP 