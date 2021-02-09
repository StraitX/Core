#ifndef STRAITX_LOGICAL_GPU_HPP
#define STRAITX_LOGICAL_GPU_HPP

#include "platform/result.hpp"
#include "core/noncopyable.hpp"
#include "core/assert.hpp"
#include "graphics/api/physical_gpu.hpp"

namespace StraitX{

class GraphicsAPILoader;

class CPUBuffer;

class LogicalGPU: NonCopyable{
private:
    static LogicalGPU *s_Instance;
    
    friend class GraphicsAPILoader;
protected:
    struct VTable{
        /*
        using NewGPUBufferProc    = void (*)(GPUBuffer &);
        using DeleteGPUBufferProc = void (*)(GPUBuffer &);

        NewGPUBufferProc    NewGPUBuffer;
        DeleteGPUBufferProc DeleteGPUBuffer;
        */

        using NewCPUBufferProc    = void (*)(CPUBuffer &buffer, size_t size);
        using DeleteCPUBufferProc = void (*)(CPUBuffer &buffer);

        NewCPUBufferProc    NewCPUBuffer;
        DeleteCPUBufferProc DeleteCPUBuffer;
    };
    VTable m_VTable;
public:

    virtual Result Initialize(const PhysicalGPU &gpu) = 0;

    virtual void Finalize() = 0;

    sx_inline void NewCPUBuffer(CPUBuffer &buffer, size_t size){
        m_VTable.NewCPUBuffer(buffer, size);
    }

    sx_inline void DeleteCPUBuffer(CPUBuffer &buffer){
        m_VTable.DeleteCPUBuffer(buffer);
    }

    sx_inline static LogicalGPU &Instance(){
        CoreAssert(s_Instance, "LogicalGPU was not loaded properly");
        return *s_Instance;
    }
};

}// namespace StraitX::

#endif //STRAITX_LOGICAL_GPU_HPP