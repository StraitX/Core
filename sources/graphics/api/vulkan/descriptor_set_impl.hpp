#ifndef STRAITX_VULKAN_DESCRIPTOR_SET_IMPL_HPP
#define STRAITX_VULKAN_DESCRIPTOR_SET_IMPL_HPP

#include "core/os/vulkan.hpp"
#include "core/fixed_list.hpp"
#include "graphics/api/descriptor_set.hpp"

namespace Vk{

class DescriptorSetLayoutImpl: public DescriptorSetLayout{
private:
	VkDescriptorSetLayout m_Handle = VK_NULL_HANDLE;
	FixedList<ShaderBinding, MaxShaderBindings> m_Bindings;
public:
	DescriptorSetLayoutImpl(ConstSpan<ShaderBinding> bindings);

	~DescriptorSetLayoutImpl();

	ConstSpan<ShaderBinding> Bindings()const override;

	operator VkDescriptorSetLayout()const{
		return m_Handle;
	}
};

class DescriptorSetImpl: public DescriptorSet{
private:
	VkDescriptorSet m_Handle = VK_NULL_HANDLE;
public:
	DescriptorSetImpl(VkDescriptorSet handle);

	void UpdateUniformBinding(size_t binding, size_t index, const Buffer *buffer)override;

	void UpdateStorageBufferBinding(size_t binding, size_t index, const Buffer *buffer)override;
	
	void UpdateTextureBinding(size_t binding, size_t index, const Texture2D *texture, const Sampler *sampler)override;

	void UpdateStorageTextureBinding(size_t binding, size_t index, const Texture2D *texture, const Sampler *sampler)override;

	operator VkDescriptorSet()const{
		return m_Handle;
	}
};

class DescriptorSetPoolImpl: public DescriptorSetPool{
private:
	VkDescriptorPool m_Handle = VK_NULL_HANDLE;
	size_t m_Capacity = 0;
	const DescriptorSetLayoutImpl *m_Layout = nullptr;
public:
	DescriptorSetPoolImpl(const DescriptorSetPoolProperties &props);

	~DescriptorSetPoolImpl();

	DescriptorSet *Alloc()override;

	void Free(DescriptorSet *set)override;

	size_t Capacity()const override;

	const DescriptorSetLayout *Layout()const override;
};

}//namespace Vk::

#endif//STRAITX_VULKAN_DESCRIPTOR_SET_IMPL_HPP