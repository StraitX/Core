#ifndef STRAITX_OPENGL_DESCRIPTOR_SET_IMPL_HPP
#define STRAITX_OPENGL_DESCRIPTOR_SET_IMPL_HPP

#include "graphics/api/descriptor_set.hpp"

namespace StraitX{
namespace GL{

struct DescriptorSetLayoutImpl{
	static void NewImpl(DescriptorSetLayout &layout);

	static void DeleteImpl(DescriptorSetLayout &layout);
};

struct VirtualBinding{
	ShaderBindingType Type;
	u32 BaseGLBinding;
	u32 ArraySize = 0;
};

struct Binding{
	size_t Binding;
	size_t Index;
};

struct UniformBufferBinding: Binding{
	u32 UniformBuffer = 0;
};

struct TextureBinding: Binding{
	u32 Texture = 0;
	u32 Sampler = 0;
};

struct DescriptorSetImpl: public DescriptorSet{
	PushArray<UniformBufferBinding, DescriptorSetLayout::s_MaxUniformBuffersBindings> UniformBufferBindings;
	PushArray<TextureBinding, DescriptorSetLayout::s_MaxTexturesBindings> TextureBindings;

	virtual void UpdateUniformBinding(size_t binding, size_t index, const GPUBuffer &buffer)override;
	
	virtual void UpdateTextureBinding(size_t binding, size_t index, const GPUTexture &texture, const Sampler &sampler)override;
};

class DescriptorSetPoolImpl: public DescriptorSetPool{
private:
	const DescriptorSetLayout *m_Layout;
public:
	DescriptorSetPoolImpl(const DescriptorSetLayout *layout, size_t pool_size);

	virtual DescriptorSet *AllocateSet()override;

	virtual void FreeSet(DescriptorSet *set)override;

	static DescriptorSetPool *NewImpl(const DescriptorSetLayout *layout, size_t pool_size);

	static void DeleteImpl(DescriptorSetPool *pool);
};


}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_DESCRIPTOR_SET_IMPL_HPP