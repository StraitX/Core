#ifndef STRAITX_VULKAN_COMPUTE_PIPELINE_IMPL_HPP
#define STRAITX_VULKAN_COMPUTE_PIPELINE_IMPL_HPP

#include "core/os/vulkan.hpp"
#include "graphics/api/compute_pipeline.hpp"
#include "graphics/api/vulkan/pipeline.hpp"

namespace Vk {

class ComputePipelineImpl : public ComputePipeline, public Pipeline{
public:
	ComputePipelineImpl(const ComputePipelineProperties &props);

	~ComputePipelineImpl();
};

}//namespace Vk::

#endif //STRAITX_VULKAN_COMPUTE_PIPELINE_IMPL_HPP