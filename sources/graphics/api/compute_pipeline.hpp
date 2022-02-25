#ifndef STRAITX_COMPUTE_PIPELINE_HPP
#define STRAITX_COMPUTE_PIPELINE_HPP

#include "core/noncopyable.hpp"
#include "graphics/api/shader.hpp"
#include "graphics/api/descriptor_set.hpp"

struct ComputePipelineProperties {
	const Shader *ComputeShader = nullptr;
	const DescriptorSetLayout *Layout = nullptr;
};

class ComputePipeline: public NonCopyable{
public:
	virtual ~ComputePipeline() = default;

	static ComputePipeline *Create(const ComputePipelineProperties &props);
};

#endif//STRAITX_COMPUTE_PIPELINE_HPP