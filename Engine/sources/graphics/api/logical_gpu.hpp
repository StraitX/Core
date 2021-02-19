#ifndef STRAITX_LOGICAL_GPU_HPP
#define STRAITX_LOGICAL_GPU_HPP

#include "platform/result.hpp"
#include "core/noncopyable.hpp"
#include "core/assert.hpp"
#include "graphics/api/physical_gpu.hpp"

namespace StraitX{

class GraphicsAPILoader;

class LogicalGPU: NonCopyable{
private:
    static LogicalGPU *s_Instance;
    
    friend class GraphicsAPILoader;
public:
    virtual Result Initialize(const PhysicalGPU &gpu) = 0;

    virtual void Finalize() = 0;

    sx_inline static LogicalGPU &Instance();
};

sx_inline LogicalGPU &LogicalGPU::Instance(){
    CoreAssert(s_Instance, "LogicalGPU was not loaded properly");
    return *s_Instance;
}

}// namespace StraitX::

#endif //STRAITX_LOGICAL_GPU_HPP