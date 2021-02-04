#ifndef STRAITX_LOGICAL_GPU_HPP
#define STRAITX_LOGICAL_GPU_HPP

#include "platform/result.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/physical_gpu.hpp"

namespace StraitX{

struct LogicalGPU: NonCopyable{

    static LogicalGPU *New(const PhysicalGPU &gpu);

    static void Delete(LogicalGPU *gpu);
};

}// namespace StraitX::

#endif //STRAITX_LOGICAL_GPU_HPP