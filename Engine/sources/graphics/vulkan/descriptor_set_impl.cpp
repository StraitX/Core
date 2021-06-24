#include "platform/memory.hpp"
#include "graphics/vulkan/descriptor_set_impl.hpp"
#include "graphics/vulkan/gpu.hpp"

namespace StraitX{
namespace Vk{

VkDescriptorType DescriptorSetLayoutImpl::s_DescriptorTypeTable[] = {
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
};

void DescriptorSetLayoutImpl::NewImpl(DescriptorSetLayout &layout){

	VkDescriptorSetLayoutBinding *bindings = SX_STACK_ARRAY_ALLOC(VkDescriptorSetLayoutBinding, layout.Size());

	for(size_t i = 0; i<layout.Size(); i++){
		bindings[i].binding = layout[i].Binding;
		bindings[i].descriptorType = s_DescriptorTypeTable[(size_t)layout[i].Type];
		bindings[i].descriptorCount = layout[i].Size;
		bindings[i].stageFlags = layout[i].VisibleShaders;
		bindings[i].pImmutableSamplers = nullptr;
	}

	VkDescriptorSetLayoutCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
	info.bindingCount = layout.Size();
	info.pBindings = bindings;

	VkDescriptorSetLayout handle = VK_NULL_HANDLE;

	CoreFunctionAssert(vkCreateDescriptorSetLayout(GPU::Get().Handle(), &info, nullptr, &handle),VK_SUCCESS, "Vk: Can't create descriptor set layout");	

	layout.m_Handle.U64 = u64(handle);
}

void DescriptorSetLayoutImpl::DeleteImpl(DescriptorSetLayout &layout){
	vkDestroyDescriptorSetLayout(GPU::Get().Handle(), VkDescriptorSetLayout(layout.m_Handle.U64), nullptr);
}

DescriptorSetImpl::DescriptorSetImpl(VkDescriptorSet handle):
	m_Handle(handle)
{}

void DescriptorSetImpl::UpdateUniformBinding(size_t binding, size_t index, const GPUBuffer &uniform_buffer){
    VkDescriptorBufferInfo buffer;
    buffer.buffer = VkBuffer(uniform_buffer.Handle().U64);
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

    vkUpdateDescriptorSets(GPU::Get().Handle(), 1, &write, 0, nullptr);
}
	
void DescriptorSetImpl::UpdateTextureBinding(size_t binding, size_t index, const GPUTexture &texture, const Sampler &sampler){
    VkDescriptorImageInfo image;
    image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;//GPUTextureImpl::s_LayoutTable[(size_t)texture.GetLayout()];
    image.imageView = VkImageView(texture.ViewHandle().U64);
    image.sampler = VkSampler(sampler.Handle().U64);

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

    vkUpdateDescriptorSets(GPU::Get().Handle(), 1, &write, 0, nullptr);
}


DescriptorSetPoolImpl::DescriptorSetPoolImpl(const DescriptorSetLayout *layout, size_t pool_size):
	m_Layout(layout)
{
	VkDescriptorPoolSize *sizes = SX_STACK_ARRAY_ALLOC(VkDescriptorPoolSize, layout->Size());

	for(size_t i = 0; i<layout->Size(); i++){
		sizes[i].descriptorCount = layout->operator[](i).Size;
		sizes[i].type = DescriptorSetLayoutImpl::s_DescriptorTypeTable[(size_t)layout->operator[](i).Type];
	}

	VkDescriptorPoolCreateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = 0;
    info.maxSets = pool_size;
    info.poolSizeCount = layout->Size();
    info.pPoolSizes = sizes;

	CoreFunctionAssert(vkCreateDescriptorPool(GPU::Get().Handle(), &info, nullptr, &m_DescriptorPool), VK_SUCCESS, "Vk: Can't create descriptor pool");
}

DescriptorSetPoolImpl::~DescriptorSetPoolImpl(){
	vkDestroyDescriptorPool(GPU::Get().Handle(), m_DescriptorPool, nullptr);
}

DescriptorSet *DescriptorSetPoolImpl::AllocateSet(){
	VkDescriptorSetLayout layout = VkDescriptorSetLayout(m_Layout->Handle().U64);

	VkDescriptorSetAllocateInfo info;
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	info.pNext = nullptr;
	info.descriptorPool = m_DescriptorPool;
	info.descriptorSetCount = 1;
	info.pSetLayouts = &layout;

	VkDescriptorSet set = VK_NULL_HANDLE;

	CoreFunctionAssert(vkAllocateDescriptorSets(GPU::Get().Handle(), &info, &set),VK_SUCCESS, "Vk: Can't allocate descriptor set");

	//TODO: use pool allocator
	return new(Memory::Alloc(sizeof(DescriptorSetImpl))) DescriptorSetImpl(set);
}

void DescriptorSetPoolImpl::FreeSet(DescriptorSet *set){
	set->~DescriptorSet();
	Memory::Free(set);
}

DescriptorSetPool *DescriptorSetPoolImpl::NewImpl(const DescriptorSetLayout *layout, size_t pool_size){
	return new(Memory::Alloc(sizeof(DescriptorSetPoolImpl))) DescriptorSetPoolImpl(layout, pool_size);
}

void DescriptorSetPoolImpl::DeleteImpl(DescriptorSetPool *pool){
	pool->~DescriptorSetPool();
	Memory::Free(pool);
}
}//namespace Vk::
}//namespace StraitX::