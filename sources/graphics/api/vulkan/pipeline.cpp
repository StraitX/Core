#include "graphics/api/vulkan/pipeline.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"

namespace Vk{

Pipeline::Pipeline(VkPipelineBindPoint bind_point, const DescriptorSetLayout* layout):
	m_BindPoint(bind_point)
{
	VkDescriptorSetLayout set_layout = *(const Vk::DescriptorSetLayoutImpl*)layout;

    VkPipelineLayoutCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.pNext = nullptr;
	info.flags = 0;
	info.setLayoutCount = 1;
    info.pSetLayouts = &set_layout;
	info.pushConstantRangeCount = 0;
    info.pPushConstantRanges = nullptr;
        
    SX_VK_ASSERT(vkCreatePipelineLayout(GPUImpl::Get(), &info, nullptr, &m_Layout), "Can't create pipeline layout");
}

Pipeline::~Pipeline(){
	vkDestroyPipelineLayout(GPUImpl::Get(), m_Layout, nullptr);
}

}//namespace Vk::
