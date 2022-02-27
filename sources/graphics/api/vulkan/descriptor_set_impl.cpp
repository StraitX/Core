#include "core/os/memory.hpp"
#include "graphics/api/vulkan/descriptor_set_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "graphics/api/vulkan/texture_impl.hpp"
#include "graphics/api/vulkan/sampler_impl.hpp"
#include "graphics/api/vulkan/buffer_impl.hpp"

namespace Vk{

VkDescriptorType ToVkDescriptorType(ShaderBindingType type){
	static const VkDescriptorType s_DescriptorTypeTable[] = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
	};

	return s_DescriptorTypeTable[(size_t)type];
}

DescriptorSetLayoutImpl::DescriptorSetLayoutImpl(ConstSpan<ShaderBinding> bindings){
	for(const ShaderBinding &binding: bindings)
		m_Bindings.Add(binding);
	
	VkDescriptorSetLayoutBinding *layout_bindings = SX_STACK_ARRAY_ALLOC(VkDescriptorSetLayoutBinding, bindings.Size());

	for(size_t i = 0; i<bindings.Size(); i++){
		layout_bindings[i].binding = m_Bindings[i].Binding;
		layout_bindings[i].descriptorType = ToVkDescriptorType(m_Bindings[i].Type);
		layout_bindings[i].descriptorCount = m_Bindings[i].ElementsCount;
		layout_bindings[i].stageFlags = m_Bindings[i].VisibleStages;
		layout_bindings[i].pImmutableSamplers = nullptr;
	}

	VkDescriptorSetLayoutCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.bindingCount = bindings.Size();
	info.pBindings = layout_bindings;

	VkDescriptorSetLayout handle = VK_NULL_HANDLE;

	vkCreateDescriptorSetLayout(GPUImpl::s_Instance, &info, nullptr, &m_Handle);	
}

DescriptorSetLayoutImpl::~DescriptorSetLayoutImpl(){
	vkDestroyDescriptorSetLayout(GPUImpl::s_Instance, m_Handle, nullptr);
}

ConstSpan<ShaderBinding> DescriptorSetLayoutImpl::Bindings()const{
	return m_Bindings;
}

DescriptorSetImpl::DescriptorSetImpl(VkDescriptorSet handle):
	m_Handle(handle)
{}

void DescriptorSetImpl::UpdateUniformBinding(size_t binding, size_t index, const Buffer *uniform_buffer){
	SX_CORE_ASSERT(uniform_buffer->MemoryType() == BufferMemoryType::DynamicVRAM || uniform_buffer->MemoryType() == BufferMemoryType::VRAM, "Can't use RAM-based buffer in descriptor set");
    
	VkDescriptorBufferInfo buffer;
    buffer.buffer = *(Vk::BufferImpl*)uniform_buffer;
    buffer.offset = 0;
    buffer.range = VK_WHOLE_SIZE;

    VkWriteDescriptorSet write;
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;
    write.dstSet = m_Handle;
    write.dstBinding = binding;
    write.dstArrayElement = index;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.pImageInfo = nullptr;
    write.pBufferInfo = &buffer;
    write.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(GPUImpl::s_Instance, 1, &write, 0, nullptr);
}
void DescriptorSetImpl::UpdateStorageBufferBinding(size_t binding, size_t index, const Buffer* storage_buffer) {
	VkDescriptorBufferInfo buffer;
    buffer.buffer = *(Vk::BufferImpl*)storage_buffer;
    buffer.offset = 0;
    buffer.range = VK_WHOLE_SIZE;

    VkWriteDescriptorSet write;
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;
    write.dstSet = m_Handle;
    write.dstBinding = binding;
    write.dstArrayElement = index;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    write.pImageInfo = nullptr;
    write.pBufferInfo = &buffer;
    write.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(GPUImpl::s_Instance, 1, &write, 0, nullptr);
}
	
void DescriptorSetImpl::UpdateTextureBinding(size_t binding, size_t index, const Texture2D *texture, const Sampler *sampler){
    VkDescriptorImageInfo image;
    image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;//GPUTextureImpl::s_LayoutTable[(size_t)texture.GetLayout()];
    image.imageView = ((const Vk::Texture2DImpl*)texture)->ViewHandle();
    image.sampler = *(const Vk::SamplerImpl*)sampler;

    VkWriteDescriptorSet write;
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;
    write.dstSet = m_Handle;
    write.dstBinding = binding;
    write.dstArrayElement = index;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.pImageInfo = &image;
    write.pBufferInfo = nullptr;
    write.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(GPUImpl::s_Instance, 1, &write, 0, nullptr);
}

void DescriptorSetImpl::UpdateStorageTextureBinding(size_t binding, size_t index, const Texture2D *texture, const Sampler *sampler){
    VkDescriptorImageInfo image;
    image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;//GPUTextureImpl::s_LayoutTable[(size_t)texture.GetLayout()];
    image.imageView = ((const Vk::Texture2DImpl*)texture)->ViewHandle();
    image.sampler = *(const Vk::SamplerImpl*)sampler;

    VkWriteDescriptorSet write;
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;
    write.dstSet = m_Handle;
    write.dstBinding = binding;
    write.dstArrayElement = index;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    write.pImageInfo = &image;
    write.pBufferInfo = nullptr;
    write.pTexelBufferView = nullptr;

    vkUpdateDescriptorSets(GPUImpl::s_Instance, 1, &write, 0, nullptr);
}

DescriptorSetPoolImpl::DescriptorSetPoolImpl(const DescriptorSetPoolProperties &props):
	m_Capacity(props.Capacity),
	m_Layout((const Vk::DescriptorSetLayoutImpl*)props.Layout)
{
	VkDescriptorPoolSize *sizes = SX_STACK_ARRAY_ALLOC(VkDescriptorPoolSize, m_Layout->Bindings().Size());

	for(size_t i = 0; i<m_Layout->Bindings().Size(); i++){
		sizes[i].descriptorCount = m_Layout->Bindings()[i].ElementsCount;
		sizes[i].type = ToVkDescriptorType(m_Layout->Bindings()[i].Type);
	}

	VkDescriptorPoolCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    info.maxSets = m_Capacity;
    info.poolSizeCount = m_Layout->Bindings().Size();
    info.pPoolSizes = sizes;

	vkCreateDescriptorPool(GPUImpl::s_Instance, &info, nullptr, &m_Handle);
}

DescriptorSetPoolImpl::~DescriptorSetPoolImpl(){
	vkDestroyDescriptorPool(GPUImpl::s_Instance, m_Handle, nullptr);
}

DescriptorSet *DescriptorSetPoolImpl::Alloc(){
	VkDescriptorSetLayout layout = *m_Layout;

	VkDescriptorSetAllocateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	info.pNext = nullptr;
	info.descriptorPool = m_Handle;
	info.descriptorSetCount = 1;
	info.pSetLayouts = &layout;

	VkDescriptorSet set = VK_NULL_HANDLE;

	SX_VK_ASSERT(vkAllocateDescriptorSets(GPUImpl::s_Instance, &info, &set), "Vk: Can't allocate descriptor set");

	//TODO: use pool allocator
	return new DescriptorSetImpl(set);
}

void DescriptorSetPoolImpl::Free(DescriptorSet *set){
	VkDescriptorSet handle = *(Vk::DescriptorSetImpl*)set;
	vkFreeDescriptorSets(GPUImpl::s_Instance, m_Handle, 1, &handle);
	delete set;
}

size_t DescriptorSetPoolImpl::Capacity()const{
	return m_Capacity;
}

const DescriptorSetLayout *DescriptorSetPoolImpl::Layout()const{
	return m_Layout;
}

}//namespace Vk::