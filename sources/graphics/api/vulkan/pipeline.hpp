#ifndef STRAITX_VULKAN_PIPELINE_HPP
#define STRAITX_VULKAN_PIPELINE_HPP

#include "core/os/vulkan.hpp"
#include "graphics/api/vulkan/descriptor_set_impl.hpp"

namespace Vk {

class Pipeline {
protected:
	VkPipeline m_Handle = VK_NULL_HANDLE;
	VkPipelineLayout m_Layout = VK_NULL_HANDLE;
	const VkPipelineBindPoint m_BindPoint;
public:
	Pipeline(VkPipelineBindPoint bind_point, const DescriptorSetLayout *layout);

	~Pipeline();

	operator VkPipeline()const{
		return m_Handle;
	}

	VkPipelineLayout Layout()const {
		return m_Layout;
	}

	VkPipelineBindPoint BindPoint() const {
		return m_BindPoint;
	}
};

}//namespace Vk::

#endif//STRAITX_VULKAN_PIPELINE_HPP