#ifndef STRAITX_LOGICAL_GPU_HPP
#define STRAITX_LOGICAL_GPU_HPP

#include "platform/result.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/physical_gpu.hpp"

namespace StraitX{

class LogicalGPU: NonCopyable{
    static Result Initialize(const PhysicalGPU &gpu);

    static void Finalize();
};

}// namespace StraitX::

#endif //STRAITX_LOGICAL_GPU_HPP