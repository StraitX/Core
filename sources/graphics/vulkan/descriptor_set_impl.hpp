#ifndef STRAITX_VULKAN_DESCRIPTOR_SET_IMPL_HPP
#define STRAITX_VULKAN_DESCRIPTOR_SET_IMPL_HPP

#include "core/os/vulkan.hpp"
#include "graphics/descriptor_set.hpp"

namespace Vk{

struct DescriptorSetLayoutImpl{
	static VkDescriptorType s_DescriptorTypeTable[];

	static void NewImpl(DescriptorSetLayout &layout);

	static void DeleteImpl(DescriptorSetLayout &layout);
};

class DescriptorSetImpl: public DescriptorSet{
private:
	VkDescriptorSet m_Handle = VK_NULL_HANDLE;
public:
	DescriptorSetImpl(VkDescriptorSet handle);

	virtual void UpdateUniformBinding(size_t binding, size_t index, const GPUBuffer &buffer)override;
	
	virtual void UpdateTextureBinding(size_t binding, size_t index, const Texture2D &texture)override;

	VkDescriptorSet Handle()const;
};

SX_INLINE VkDescriptorSet DescriptorSetImpl::Handle()const{
	return m_Handle;
}

class DescriptorSetPoolImpl: public DescriptorSetPool{
private:
	VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
	const DescriptorSetLayout *m_Layout;
public:
	DescriptorSetPoolImpl(const DescriptorSetLayout *layout, size_t pool_size);

	~DescriptorSetPoolImpl();

	virtual DescriptorSet *AllocateSet()override;

	virtual void FreeSet(DescriptorSet *set)override;

	static DescriptorSetPool *NewImpl(const DescriptorSetLayout *layout, size_t pool_size);

	static void DeleteImpl(DescriptorSetPool *pool);
};

}//namespace Vk::

#endif//STRAITX_VULKAN_DESCRIPTOR_SET_IMPL_HPP