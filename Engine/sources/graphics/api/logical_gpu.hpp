#ifndef STRAITX_LOGICAL_GPU_HPP
#define STRAITX_LOGICAL_GPU_HPP

#include "platform/result.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/physical_gpu.hpp"

namespace StraitX{

class LogicalGPU: public NonCopyable{
public:

    virtual Result Create(const PhysicalGPU &gpu) = 0;

    virtual void Destroy() = 0;

    static LogicalGPU *New();

    static void Delete(LogicalGPU *gpu);

};

}// namespace StraitX::

#endif //STRAITX_LOGICAL_GPU_HPP