#include "platform/memory.hpp"
#include "platform/opengl.hpp"
#include "core/algorithm.hpp"
#include "graphics/opengl/descriptor_set_impl.hpp"

namespace GL{

void DescriptorSetLayoutImpl::NewImpl(DescriptorSetLayout &layout){
	(void)layout;
}

void DescriptorSetLayoutImpl::DeleteImpl(DescriptorSetLayout &layout){
	(void)layout;
}

void DescriptorSetImpl::UpdateUniformBinding(size_t binding, size_t index, const GPUBuffer &buffer){
	auto uniform_binding = FindIf(UniformBufferBindings.begin(), UniformBufferBindings.end(), [binding, index](const UniformBufferBinding &ubb){
		return ubb.Binding == binding && ubb.Index == index;
	});

	if(uniform_binding == UniformBufferBindings.end()){
		UniformBufferBindings.Emplace();
		uniform_binding->Binding = binding;
		uniform_binding->Index = index;
	}
	uniform_binding->UniformBuffer = buffer.Handle().U32;
}
	
void DescriptorSetImpl::UpdateTextureBinding(size_t binding, size_t index, const Texture2D &texture){
	auto texture_binding = FindIf(TextureBindings.begin(), TextureBindings.end(), [binding, index](const TextureBinding &tb){
		return tb.Binding == binding && tb.Index == index;
	});

	if(texture_binding == TextureBindings.end()){
		TextureBindings.Emplace();
		texture_binding->Binding = binding;
		texture_binding->Index = index;
	}
	texture_binding->Texture = texture.Handle().U32;
	texture_binding->Sampler = texture.Sampler().Handle().U32;
}

DescriptorSetPoolImpl::DescriptorSetPoolImpl(const DescriptorSetLayout *layout, size_t pool_size):
	m_Layout(layout)
{
	(void)pool_size;
}

DescriptorSet *DescriptorSetPoolImpl::AllocateSet(){
	//XXX: use pool allocator
	return new (Memory::Alloc(sizeof(DescriptorSetImpl))) DescriptorSetImpl();
}

void DescriptorSetPoolImpl::FreeSet(DescriptorSet *set){
	//XXX: use pool allocator
	set->~DescriptorSet();
	Memory::Free(set);
}

DescriptorSetPool *DescriptorSetPoolImpl::NewImpl(const DescriptorSetLayout *layout, size_t pool_size){
	return new (Memory::Alloc(sizeof(DescriptorSetPoolImpl))) DescriptorSetPoolImpl(layout, pool_size);
}

void DescriptorSetPoolImpl::DeleteImpl(DescriptorSetPool *pool){
	pool->~DescriptorSetPool();
	Memory::Free(pool);
}

}//namespace GL::