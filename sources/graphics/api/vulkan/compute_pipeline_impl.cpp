#include "graphics/api/vulkan/compute_pipeline_impl.hpp"
#include "graphics/api/vulkan/descriptor_set_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "graphics/api/vulkan/shader_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"

namespace Vk{

ComputePipelineImpl::ComputePipelineImpl(const ComputePipelineProperties& props):
    Pipeline(VK_PIPELINE_BIND_POINT_COMPUTE, props.Layout)
{
    SX_CORE_ASSERT(props.ComputeShader->Stage() == ShaderStageBits::Compute, "Can't create pipeline from non-compute shader");
    {
        VkPipelineShaderStageCreateInfo stage;
        stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage.pNext = nullptr;
		stage.flags = 0;
		stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		stage.module = *(const Vk::ShaderImpl*)props.ComputeShader;
        stage.pName = "main";
        stage.pSpecializationInfo = nullptr;

	    VkComputePipelineCreateInfo info;
	    info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	    info.pNext = nullptr;
        info.flags = 0;
        info.stage = stage;
        info.layout = m_Layout;
        info.basePipelineHandle = VK_NULL_HANDLE;
        info.basePipelineIndex = -1;

        SX_VK_ASSERT(vkCreateComputePipelines(GPUImpl::Get(), VK_NULL_HANDLE, 1, &info, nullptr, &m_Handle), "Can't create ComputePipeline");
    }
}

ComputePipelineImpl::~ComputePipelineImpl(){
    vkDestroyPipeline(GPUImpl::Get(), m_Handle, nullptr);
}

}//namespace Vk::
