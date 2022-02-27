#ifndef STRAITX_DESCRIPTOR_SET_HPP
#define STRAITX_DESCRIPTOR_SET_HPP

#include "core/env/compiler.hpp"
#include "core/fixed_list.hpp"
#include "core/list.hpp"
#include "graphics/api/shader.hpp"
#include "graphics/api/buffer.hpp"
#include "graphics/api/texture.hpp"
#include "graphics/api/shader.hpp"
#include "graphics/api/sampler.hpp"

enum class ShaderBindingType: u8{
    UniformBuffer = 0,
    StorageBuffer = 1,
    Texture       = 2,
    StorageTexture= 3,
};

struct ShaderBinding{
    u32 Binding;
    u32 ElementsCount;
    ShaderBindingType Type;
    ShaderStage VisibleStages;

    constexpr ShaderBinding(u32 binding, u32 elements_count, ShaderBindingType type, ShaderStage visible_stages):
        Binding(binding),
        ElementsCount(elements_count),
        Type(type),
        VisibleStages(visible_stages)
    {}
};

// OpenGL supports at least 16 units, and we use last one as a default binding point
constexpr size_t MaxTexturesBindings = 16 - 1; 
// this value has nothing to do with the spec
constexpr size_t MaxUniformBuffersBindings = 16;

constexpr size_t MaxShaderBindings = MaxTexturesBindings + MaxUniformBuffersBindings;

class DescriptorSetLayout: public NonCopyable{
public:
	DescriptorSetLayout() = default;

	virtual ~DescriptorSetLayout() = default;

	virtual ConstSpan<ShaderBinding> Bindings()const = 0;

	static DescriptorSetLayout *Create(ConstSpan<ShaderBinding> bindings);
};

class DescriptorSet: public NonCopyable{
public:
	DescriptorSet() = default;

	virtual ~DescriptorSet() = default;

	virtual void UpdateUniformBinding(size_t binding, size_t index, const Buffer *buffer) = 0;

	virtual void UpdateStorageBufferBinding(size_t binding, size_t index, const Buffer *buffer) = 0;
	
	virtual void UpdateTextureBinding(size_t binding, size_t index, const Texture2D *texture, const Sampler *sampler) = 0;

	virtual void UpdateStorageTextureBinding(size_t binding, size_t index, const Texture2D *texture, const Sampler *sampler) = 0;
};

struct DescriptorSetPoolProperties{
	size_t Capacity = 0;
	const DescriptorSetLayout *Layout = nullptr;
};

class DescriptorSetPool: public NonCopyable{
public:
	DescriptorSetPool() = default;

	virtual ~DescriptorSetPool() = default;

	virtual DescriptorSet *Alloc() = 0;

	virtual void Free(DescriptorSet *set) = 0;

	virtual size_t Capacity()const = 0;

	virtual const DescriptorSetLayout *Layout()const = 0;

	static DescriptorSetPool *Create(const DescriptorSetPoolProperties &props);
};

struct DescriptorSetDeleter {
	DescriptorSetPool *Pool = nullptr;

	DescriptorSetDeleter(DescriptorSetPool *pool):
		Pool(pool)
	{}

	void operator()(DescriptorSet* set) {
		Pool->Free(set);
	}
};

class SingleFrameDescriptorSetPool{
private:
	DescriptorSetPool *m_Pool = nullptr;
	List<DescriptorSet*> m_AllocatedSets;
	size_t m_FreePointer = 0;
public:
	SingleFrameDescriptorSetPool(const DescriptorSetPoolProperties &props, size_t preallocate_sets = 0);

	~SingleFrameDescriptorSetPool();

	DescriptorSet *Alloc();

	void NextFrame();
};

#endif//STRAITX_DESCRIPTOR_SET_HPP