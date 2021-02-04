#ifndef STRAITX_LOGICAL_GPU_HPP
#define STRAITX_LOGICAL_GPU_HPP

#include "platform/result.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/physical_gpu.hpp"

namespace StraitX{

class LogicalGPU: NonCopyable{
private:
    static LogicalGPU *s_Instance;
public:
    static Result Initialize(const PhysicalGPU &gpu);

    static void Finalize();

    static sx_inline LogicalGPU &Instance(){ return *s_Instance; }

};

}// namespace StraitX::

#endif //STRAITX_LOGICAL_GPU_HPP