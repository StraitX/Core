#ifndef STRAITX_DESCRIPTOR_SET_HPP
#define STRAITX_DESCRIPTOR_SET_HPP

#include "platform/compiler.hpp"
#include "core/push_array.hpp"
#include "graphics/api/shader.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/texture.hpp"
#include "graphics/api/sampler.hpp"

enum class ShaderBindingType: u8{
    UniformBuffer = 0,
    Texture       = 1
};

struct ShaderBinding{
    u32 Binding;
    u32 Size;
    ShaderBindingType Type;
    Shader::Types VisibleShaders;

    constexpr ShaderBinding(u32 binding, u32 size, ShaderBindingType type, Shader::Types visible_shaders):
        Binding(binding),
        Size(size),
        Type(type),
        VisibleShaders(visible_shaders)
    {}
};

namespace Vk{
class DescriptorSetLayoutImpl;
}//namespace Vk::
namespace GL{
class DescriptorSetLayoutImpl;
}//namespace GL::

class DescriptorSetLayout: public NonCopyable{
public:
	struct VTable{
		using NewProc    = void (*)(DescriptorSetLayout &layout);
		using DeleteProc = void (*)(DescriptorSetLayout &layout);

		NewProc    New    = nullptr;
		DeleteProc Delete = nullptr;
	};
public:
	// OpenGL supports at least 16 units, and we use last one as a default binding point
	static constexpr size_t s_MaxTexturesBindings = 16 - 1; 
	// this value has nothing to do with the spec
	static constexpr size_t s_MaxUniformBuffersBindings = 16;
	
	static constexpr size_t s_MaxBindings = s_MaxTexturesBindings + s_MaxUniformBuffersBindings;
private:
	static VTable s_VTable;

	friend class GL::DescriptorSetLayoutImpl;
	friend class Vk::DescriptorSetLayoutImpl;
	friend class GraphicsAPILoader;
private:
	PushArray<ShaderBinding, s_MaxBindings> m_Bindings;
	GPUResourceHandle m_Handle;
public:
	DescriptorSetLayout(Span<ShaderBinding> bindings);

	~DescriptorSetLayout();

	size_t Size()const;

	const ShaderBinding &operator[](size_t index)const;

	GPUResourceHandle Handle()const;
};

SX_INLINE DescriptorSetLayout::DescriptorSetLayout(Span<ShaderBinding> bindings){
	for(const auto &binding: bindings)
		m_Bindings.Push(binding);
	
	s_VTable.New(*this);
}

SX_INLINE DescriptorSetLayout::~DescriptorSetLayout(){
	s_VTable.Delete(*this);
}

SX_INLINE size_t DescriptorSetLayout::Size()const{
	return m_Bindings.Size();
}

SX_INLINE const ShaderBinding &DescriptorSetLayout::operator[](size_t index)const{
	return m_Bindings[index];
}

SX_INLINE GPUResourceHandle DescriptorSetLayout::Handle()const{
	return m_Handle;
}

class DescriptorSet: public NonCopyable{
public:
	virtual ~DescriptorSet() = default;

	virtual void UpdateUniformBinding(size_t binding, size_t index, const GPUBuffer &buffer) = 0;
	
	virtual void UpdateTextureBinding(size_t binding, size_t index, const Texture2D &texture) = 0;
};

class DescriptorSetPool: public NonCopyable{
public:
	struct VTable{
		using NewProc    = DescriptorSetPool *(*)(const DescriptorSetLayout *layout, size_t pool_size);
		using DeleteProc = void (*)(DescriptorSetPool *pool);

		NewProc    New    = nullptr;
		DeleteProc Delete = nullptr;
	};
private:
	static VTable s_VTable;

	friend class GraphicsAPILoader;
public:
	virtual ~DescriptorSetPool() = default;

	virtual DescriptorSet *AllocateSet() = 0;

	virtual void FreeSet(DescriptorSet *set) = 0;

	static DescriptorSetPool *New(const DescriptorSetLayout *layout, size_t pool_size);

	static void Delete(DescriptorSetPool *pool);
};

SX_INLINE DescriptorSetPool *DescriptorSetPool::New(const DescriptorSetLayout *layout, size_t pool_size){
	return s_VTable.New(layout, pool_size);
}

SX_INLINE void DescriptorSetPool::Delete(DescriptorSetPool *pool){
	s_VTable.Delete(pool);
}

#endif//STRAITX_DESCRIPTOR_SET_HPP